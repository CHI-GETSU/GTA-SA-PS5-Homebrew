/* SPDX-License-Identifier: LGPL-2.1-or-later
 * R2: opt-in DirectInput bridge, late XInput availability, session health.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "device_private.h"
#include "xinput.h"
#include "wine/debug.h"
WINE_DEFAULT_DEBUG_CHANNEL(pwpad);

static const GUID controller_guid = {0x70773535,0x4453,0x4455,{0x80,0x20,0x50,0x57,0,0,0,1}};
static const GUID product_guid = {0x0ce6054c,0,0,{0,0,0x50,0x49,0x44,0x56,0x49,0x44}};
static const struct dinput_device_vtbl joystick_vtbl;
struct xinput_joystick {
    struct dinput_device base;
    HMODULE module;
    DWORD (WINAPI *get_state)(DWORD, XINPUT_STATE *);
    BOOL acquired, gta_ps2;
    DWORD last_result, next_log, next_health, next_error;
    HRESULT last_acquire;
    BOOL acquire_logged;
    unsigned long long state_calls, health_sequence;
    XINPUT_STATE state;
};
static BOOL enabled(void) {
    WCHAR text[2];
    return GetEnvironmentVariableW(L"PW_DINPUT_XINPUT",text,ARRAY_SIZE(text)) == 1 && text[0] == '1';
}
void xinput_joystick_diag_attach(HMODULE module) {
    WCHAR path[MAX_PATH], flag[16] = L"", session[64] = L"";
    GetModuleFileNameW(module,path,ARRAY_SIZE(path));
    GetEnvironmentVariableW(L"PW_DINPUT_XINPUT",flag,ARRAY_SIZE(flag));
    GetEnvironmentVariableW(L"PW_DINPUT_SESSION",session,ARRAY_SIZE(session));
    ERR("PW_PADDIAG R2 attach pe_bits=%u path=%s enabled=%s session=%s\n",
        (unsigned)(8*sizeof(void *)),debugstr_w(path),debugstr_w(flag),debugstr_w(session));
}
/* GTA consumes POV as left-stick motion; PS2 D-pad actions use buttons 13..16. */
static BOOL gta_ps2(void) {
    WCHAR text[2];
    return GetEnvironmentVariableW(L"PW_DINPUT_GTA_PS2",text,ARRAY_SIZE(text)) == 1 && text[0] == '1';
}
static HMODULE open_xinput(DWORD (WINAPI **get_state)(DWORD,XINPUT_STATE *)) {
    WCHAR path[MAX_PATH];
    HMODULE module = LoadLibraryW(L"xinput1_3.dll");
    *get_state = module ? (void *)GetProcAddress(module,"XInputGetState") : NULL;
    if (module) { GetModuleFileNameW(module,path,ARRAY_SIZE(path));
        ERR("PW_PADDIAG R2 xinput module=%s get_state=%p\n",debugstr_w(path),*get_state); }
    else ERR("PW_PADDIAG R2 xinput LoadLibrary failed error=%lu\n",GetLastError());
    if (module && !*get_state) { FreeLibrary(module); module=NULL; }
    return module;
}
static void fill_instance(DIDEVICEINSTANCEW *instance,DWORD version) {
    DWORD size=instance->dwSize;
    memset(instance,0,size); instance->dwSize=size;
    instance->guidInstance=controller_guid; instance->guidProduct=product_guid;
    instance->dwDevType=version>=0x800 ? DI8DEVTYPE_GAMEPAD | (DI8DEVTYPEGAMEPAD_STANDARD<<8) :
                                       DIDEVTYPE_JOYSTICK | (DIDEVTYPEJOYSTICK_GAMEPAD<<8);
    lstrcpyW(instance->tszInstanceName,L"Prospero DualSense R2");
    lstrcpyW(instance->tszProductName,L"Prospero DualSense R2");
    instance->wUsagePage=1; instance->wUsage=5;
}
HRESULT xinput_joystick_enum_device(DWORD type,DWORD flags,DIDEVICEINSTANCEW *instance,DWORD version) {
    DWORD (WINAPI *get_state)(DWORD,XINPUT_STATE *);
    XINPUT_STATE state={0}; HMODULE module; DWORD result;
    ERR("PW_PADDIAG R2 enum enabled=%u type=%#lx flags=%#lx version=%#lx\n",enabled(),type,flags,version);
    if (!enabled() || (flags&DIEDFL_FORCEFEEDBACK)) return DIERR_DEVICENOTREG;
    if (!(module=open_xinput(&get_state))) return DIERR_DEVICENOTREG;
    result=get_state(0,&state); FreeLibrary(module);
    /* Slot is stable even if the native frame loop has not published its
     * first state. Never permanently drop the device on an early 1167. */
    fill_instance(instance,version);
    ERR("PW_PADDIAG R2 enum slot=0 initial_xinput=%lu packet=%lu (late state allowed)\n",result,state.dwPacketNumber);
    return DI_OK;
}
static struct xinput_joystick *from_iface(IDirectInputDevice8W *iface) {
    return CONTAINING_RECORD(iface,struct xinput_joystick,base.IDirectInputDevice8W_iface);
}
static void object_instance(UINT index,DIDEVICEOBJECTINSTANCEW *object) {
    static const GUID *axes[]={&GUID_XAxis,&GUID_YAxis,&GUID_ZAxis,&GUID_RxAxis,&GUID_RyAxis,&GUID_RzAxis};
    static const WCHAR *names[]={L"Left X",L"Left Y",L"Right X",L"L2 analog",L"R2 analog",L"Right Y"};
    memset(object,0,sizeof(*object)); object->dwSize=sizeof(*object); object->wReportId=1;
    if (index<6) {
        object->guidType=*axes[index]; object->dwOfs=index*sizeof(LONG);
        object->dwType=DIDFT_ABSAXIS|DIDFT_MAKEINSTANCE(index); object->dwFlags=DIDOI_ASPECTPOSITION;
        object->wUsagePage=1; object->wUsage=0x30+index; lstrcpyW(object->tszName,names[index]);
    } else if (index==6) {
        object->guidType=GUID_POV; object->dwOfs=DIJOFS_POV(0); object->dwType=DIDFT_POV;
        object->wUsagePage=1; object->wUsage=0x39; lstrcpyW(object->tszName,L"D-pad");
    } else {
        object->guidType=GUID_Button; object->dwOfs=DIJOFS_BUTTON(index-7);
        object->dwType=DIDFT_PSHBUTTON|DIDFT_MAKEINSTANCE(index-7); object->wUsagePage=9; object->wUsage=index-6;
        swprintf(object->tszName,ARRAY_SIZE(object->tszName),L"Button %u",index-6);
    }
}
static HRESULT enum_objects(IDirectInputDevice8W *iface,const DIPROPHEADER *filter,DWORD flags,
                            enum_object_callback callback,void *context) {
    struct xinput_joystick *impl=from_iface(iface); UINT i;
    for(i=0;i<23;++i) {
        DIDEVICEOBJECTINSTANCEW object; object_instance(i,&object);
        if(flags!=DIDFT_ALL && !(flags&DIDFT_GETTYPE(object.dwType))) continue;
        switch(filter->dwHow) {
        case DIPH_DEVICE: break;
        case DIPH_BYOFFSET: if(filter->dwObj!=object.dwOfs) continue; break;
        case DIPH_BYID: if((filter->dwObj&0xffffff)!=(object.dwType&0xffffff)) continue; break;
        case DIPH_BYUSAGE: if(HIWORD(filter->dwObj)!=object.wUsagePage || LOWORD(filter->dwObj)!=object.wUsage) continue; break;
        default: return DIERR_INVALIDPARAM;
        }
        if(callback(&impl->base,i,NULL,&object,context)==DIENUM_STOP) break;
    }
    return DI_OK;
}
static LONG scale_axis(LONG raw,const struct object_properties *p) {
    LONG centre=p->range_min+((LONGLONG)p->range_max-p->range_min)/2;
    LONG magnitude=raw<0?-raw:raw, extent=raw<0?32768:32767;
    LONG dead=MulDiv(extent,p->deadzone,10000), saturation=MulDiv(extent,p->saturation,10000);
    LONG end=raw<0?p->range_min:p->range_max;
    if(magnitude<=dead) return centre;
    if(magnitude>=saturation || saturation<=dead) return end;
    return centre+((LONGLONG)end-centre)*(magnitude-dead)/(saturation-dead);
}
static void store_value(struct xinput_joystick *impl,UINT index,DWORD value,DWORD time,DWORD seq) {
    DIDEVICEOBJECTINSTANCEW object; DWORD old; object_instance(index,&object);
    if(index>=7) { old=impl->base.device_state[object.dwOfs]; impl->base.device_state[object.dwOfs]=value; }
    else { memcpy(&old,impl->base.device_state+object.dwOfs,4); memcpy(impl->base.device_state+object.dwOfs,&value,4); }
    if(old!=value) queue_event(&impl->base.IDirectInputDevice8W_iface,index,value,time,seq);
}
static HRESULT poll(IDirectInputDevice8W *iface) {
    static const WORD buttons[]={XINPUT_GAMEPAD_B,XINPUT_GAMEPAD_A,XINPUT_GAMEPAD_X,XINPUT_GAMEPAD_Y,0,0,
        XINPUT_GAMEPAD_LEFT_SHOULDER,XINPUT_GAMEPAD_RIGHT_SHOULDER,XINPUT_GAMEPAD_BACK,
        XINPUT_GAMEPAD_LEFT_THUMB,XINPUT_GAMEPAD_RIGHT_THUMB,XINPUT_GAMEPAD_START,
        XINPUT_GAMEPAD_DPAD_UP,XINPUT_GAMEPAD_DPAD_RIGHT,XINPUT_GAMEPAD_DPAD_DOWN,XINPUT_GAMEPAD_DPAD_LEFT};
    struct xinput_joystick *impl=from_iface(iface);
    XINPUT_STATE state={0}; LONG axes[6],pov=-1,x,y; UINT i;
    DWORD time=GetTickCount(),rc=impl->get_state(0,&state),seq;
    EnterCriticalSection(&impl->base.crit);
    if(rc!=impl->last_result || !impl->next_log || (LONG)(time-impl->next_log)>=0) {
        ERR("PW_PADDIAG R2 poll xinput=%lu packet=%lu buttons=%04x sticks=%d,%d/%d,%d triggers=%u,%u consumed=%llu\n",
            rc,state.dwPacketNumber,state.Gamepad.wButtons,state.Gamepad.sThumbLX,state.Gamepad.sThumbLY,
            state.Gamepad.sThumbRX,state.Gamepad.sThumbRY,state.Gamepad.bLeftTrigger,state.Gamepad.bRightTrigger,impl->state_calls);
        impl->next_log=time+3000; impl->last_result=rc;
    }
    if(rc) {
        BOOL unhook=impl->acquired; impl->acquired=FALSE; impl->base.status=STATUS_UNPLUGGED;
        memset(impl->base.device_state,0,sizeof(impl->base.device_state));
        LeaveCriticalSection(&impl->base.crit);
        if(unhook) dinput_hooks_unacquire_device(iface);
        return DIERR_INPUTLOST;
    }
    if (!impl->state_calls && impl->gta_ps2)
        ERR("PW_PS2_R4 bridge pe_bits=%u pov=neutral buttons=PS2-standard axes=X,Y,Z,Rz triggers=Rx,Ry\n", (unsigned)(8*sizeof(void *)));
    impl->state=state;
    axes[0]=state.Gamepad.sThumbLX; axes[1]=state.Gamepad.sThumbLY?-1-state.Gamepad.sThumbLY:0;
    axes[2]=state.Gamepad.sThumbRX; axes[5]=state.Gamepad.sThumbRY?-1-state.Gamepad.sThumbRY:0;
    axes[3]=state.Gamepad.bLeftTrigger*257-32768; axes[4]=state.Gamepad.bRightTrigger*257-32768;
    seq=InterlockedIncrement((LONG *)&impl->base.dinput->evsequence);
    for(i=0;i<6;++i) store_value(impl,i,scale_axis(axes[i],impl->base.object_properties+i),time,seq);
    x=!!(state.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_RIGHT)-!!(state.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_LEFT);
    y=!!(state.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_DOWN)-!!(state.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_UP);
    if(y<0) pov=x<0?31500:x>0?4500:0; else if(y>0) pov=x<0?22500:x>0?13500:18000;
    else if(x) pov=x<0?27000:9000;
    if (impl->gta_ps2) pov=-1;
    store_value(impl,6,pov,time,seq);
    for(i=0;i<16;++i) { BOOL pressed=!!(state.Gamepad.wButtons&buttons[i]);
        if(i==4) pressed=state.Gamepad.bLeftTrigger>30; if(i==5) pressed=state.Gamepad.bRightTrigger>30;
        store_value(impl,i+7,pressed?0x80:0,time,seq); }
    LeaveCriticalSection(&impl->base.crit); return DI_OK;
}
void xinput_joystick_state_consumed(IDirectInputDevice8W *iface,HRESULT hr,DWORD size) {
    struct dinput_device *base=CONTAINING_RECORD(iface,struct dinput_device,IDirectInputDevice8W_iface);
    struct xinput_joystick *impl; DWORD time=GetTickCount();
    if(base->vtbl!=&joystick_vtbl) return;
    impl=from_iface(iface);
    if(FAILED(hr)) {
        if(!impl->next_error || (LONG)(time-impl->next_error)>=0) {
            ERR("PW_PADDIAG R2 GetDeviceState failed hr=%#lx size=%lu\n",hr,size);
            impl->next_error=time+3000;
        }
        return;
    }
    impl->state_calls++;
    if(!impl->next_health || (LONG)(time-impl->next_health)>=0) {
        char session[64],record[128]; DWORD length,written; HANDLE file;
        impl->next_health=time+400;
        length=GetEnvironmentVariableA("PW_DINPUT_SESSION",session,sizeof(session));
        if(!length || length>=sizeof(session)) { ERR("PW_PADDIAG R2 health no session; keyboard fallback stays active\n"); return; }
        impl->health_sequence=GetTickCount64()+1;
        length=snprintf(record,sizeof(record),"PWDI2 %s %llu\n",session,impl->health_sequence);
        file=CreateFileW(L"C:\\pw-pad-r2.health",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,0,NULL);
        if(file==INVALID_HANDLE_VALUE) { ERR("PW_PADDIAG R2 health open failed error=%lu\n",GetLastError()); return; }
        if(!WriteFile(file,record,length,&written,NULL) || written!=length) ERR("PW_PADDIAG R2 health write failed error=%lu\n",GetLastError());
        CloseHandle(file);
    }
}
static HRESULT acquire(IDirectInputDevice8W *iface) {
    HRESULT hr=poll(iface); if(SUCCEEDED(hr)) from_iface(iface)->acquired=TRUE;
    if(!from_iface(iface)->acquire_logged || from_iface(iface)->last_acquire!=hr) {
        ERR("PW_PADDIAG R2 acquire hr=%#lx\n",hr);
        from_iface(iface)->acquire_logged=TRUE;
        from_iface(iface)->last_acquire=hr;
    }
    return hr;
}
static HRESULT unacquire(IDirectInputDevice8W *iface) { from_iface(iface)->acquired=FALSE; return DI_OK; }
static void destroy(IDirectInputDevice8W *iface) { FreeLibrary(from_iface(iface)->module); }
static HRESULT get_property(IDirectInputDevice8W *iface,DWORD property,DIPROPHEADER *header,const DIDEVICEOBJECTINSTANCEW *object) {
    if(property==(DWORD_PTR)DIPROP_VIDPID) { ((DIPROPDWORD *)header)->dwData=0x0ce6054c; return DI_OK; }
    if(property==(DWORD_PTR)DIPROP_JOYSTICKID) { ((DIPROPDWORD *)header)->dwData=0; return DI_OK; }
    return DIERR_UNSUPPORTED;
}
static const struct dinput_device_vtbl joystick_vtbl={.destroy=destroy,.poll=poll,.acquire=acquire,.unacquire=unacquire,
    .enum_objects=enum_objects,.get_property=get_property};
HRESULT xinput_joystick_create_device(struct dinput *dinput,const GUID *guid,IDirectInputDevice8W **out) {
    struct xinput_joystick *impl; HRESULT hr; UINT i;
    if(!enabled() || !IsEqualGUID(guid,&controller_guid)) return DIERR_DEVICENOTREG;
    if(!(impl=calloc(1,sizeof(*impl)))) return E_OUTOFMEMORY;
    if(!(impl->module=open_xinput(&impl->get_state))) { free(impl); return DIERR_DEVICENOTREG; }
    impl->gta_ps2=gta_ps2();
    dinput_device_init(&impl->base,&joystick_vtbl,guid,dinput);
    impl->base.dwCoopLevel=DISCL_NONEXCLUSIVE|DISCL_BACKGROUND;
    fill_instance(&impl->base.instance,dinput->dwVersion); impl->base.caps.dwDevType=impl->base.instance.dwDevType;
    impl->base.caps.dwFlags|=DIDC_POLLEDDEVICE;
    if(FAILED(hr=dinput_device_init_device_format(&impl->base.IDirectInputDevice8W_iface))) {
        IDirectInputDevice8_Release(&impl->base.IDirectInputDevice8W_iface); return hr; }
    for(i=0;i<7;++i) { struct object_properties *p=impl->base.object_properties+i;
        p->range_min=0; p->range_max=i==6?35999:65535; p->logical_min=p->physical_min=i==6?0:-32768;
        p->logical_max=p->physical_max=i==6?7:32767; p->saturation=10000;
        p->deadzone=i==3||i==4||i==6?0:800; }
    *out=&impl->base.IDirectInputDevice8W_iface;
    ERR("PW_PADDIAG R2 create ok axes=%lu buttons=%lu pov=%lu\n",impl->base.caps.dwAxes,impl->base.caps.dwButtons,impl->base.caps.dwPOVs);
    return DI_OK;
}
