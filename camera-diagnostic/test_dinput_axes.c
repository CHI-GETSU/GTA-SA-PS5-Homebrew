/* SPDX-License-Identifier: LGPL-2.1-or-later */
#define COBJMACROS
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <dinput.h>
#include <xinput.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define CHECK(c) do { if(!(c)) { printf("FAIL %d %s error=%lu\n",__LINE__,#c,GetLastError()); exit(1); } } while(0)
#define HR(c) CHECK((c)==DI_OK)
typedef struct Pad { uint32_t connected,packet; uint16_t buttons; uint8_t lt,rt; int16_t lx,ly,rx,ry; } Pad;
_Static_assert(sizeof(Pad)==20,"PS5/WoW64 pad size");
static GUID guid; static unsigned found;
static unsigned z_axis,rz_axis;
static BOOL CALLBACK axes(const DIDEVICEOBJECTINSTANCEA *o,void *unused) {
    (void)unused;
    if(IsEqualGUID(&o->guidType,&GUID_ZAxis))z_axis++;
    if(IsEqualGUID(&o->guidType,&GUID_RzAxis))rz_axis++;
    return DIENUM_CONTINUE;
}

static BOOL CALLBACK enumerate(const DIDEVICEINSTANCEA *i,void *unused) {
    (void)unused;
    if(!strcmp(i->tszInstanceName,"Prospero DualSense R2")) { found++; guid=i->guidInstance; }
    return DIENUM_CONTINUE;
}
static void publish(Pad *pad) {
    DWORD written; HANDLE file=CreateFileA("C:\\pw-test-pad.bin",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,0,NULL);
    CHECK(file!=INVALID_HANDLE_VALUE); pad->packet++;
    CHECK(WriteFile(file,pad,sizeof(*pad),&written,NULL) && written==sizeof(*pad)); CloseHandle(file);
}
int main(int argc,char **argv) {
    HMODULE dll=LoadLibraryA("dinput8.dll"),xinput=LoadLibraryA("xinput1_3.dll");
    HRESULT (WINAPI *create)(HINSTANCE,DWORD,REFIID,void **,IUnknown *)=(void *)GetProcAddress(dll,"DirectInput8Create");
    DWORD (WINAPI *getstate)(DWORD,XINPUT_STATE *)=(void *)GetProcAddress(xinput,"XInputGetState");
    IDirectInput8A *input; IDirectInputDevice8A *device;
    DIJOYSTATE2 state; XINPUT_STATE xs;
    Pad pad={0}; DWORD code;
    DIPROPRANGE range={{sizeof(range),sizeof(DIPROPHEADER),0,DIPH_DEVICE},-2000,2000};
    DIPROPDWORD prop={{sizeof(prop),sizeof(DIPROPHEADER),0,DIPH_DEVICE},0};
    int ps2=GetEnvironmentVariableA("PW_DINPUT_GTA_PS2",NULL,0)!=0;
    setvbuf(stdout,NULL,_IONBF,0); CHECK(argc==2 && dll && xinput && create && getstate);
    publish(&pad);
    HR(create(GetModuleHandleA(NULL),0x800,&IID_IDirectInput8A,(void **)&input,NULL));
    HR(IDirectInput8_EnumDevices(input,DI8DEVCLASS_GAMECTRL,enumerate,NULL,DIEDFL_ATTACHEDONLY));
    if(!strcmp(argv[1],"disabled")) { CHECK(!found); IDirectInput8_Release(input); puts("PASS disabled bridge"); return 0; }
    CHECK(found==1); /* R1 lost the slot here when initial XInput was 1167. */
    HR(IDirectInput8_CreateDevice(input,&guid,&device,NULL));
    HR(IDirectInputDevice8_SetDataFormat(device,&c_dfDIJoystick2));
    HR(IDirectInputDevice8_SetCooperativeLevel(device,NULL,DISCL_NONEXCLUSIVE|DISCL_BACKGROUND));
    HR(IDirectInputDevice8_SetProperty(device,DIPROP_RANGE,&range.diph));
    HR(IDirectInputDevice8_SetProperty(device,DIPROP_DEADZONE,&prop.diph));
    CHECK(IDirectInputDevice8_Acquire(device)==DIERR_INPUTLOST);
    if(!strcmp(argv[1],"missing-backend")) {
        pad.connected=1; publish(&pad); code=getstate(0,&xs); CHECK(code!=0);
        CHECK(IDirectInputDevice8_GetDeviceState(device,sizeof(state),&state)!=DI_OK);
        IDirectInputDevice8_Release(device); IDirectInput8_Release(input);
        puts("PASS missing native sink: stable slot, API failure visible, no health success"); return 0;
    }
    pad.connected=1; publish(&pad);
    CHECK(getstate(0,&xs)==0 && xs.dwPacketNumber==pad.packet);
    HR(IDirectInputDevice8_Acquire(device));
    HR(IDirectInputDevice8_GetDeviceState(device,sizeof(state),&state));
    CHECK(state.lX==0 && state.lY==0 && state.lZ==0 && state.lRz==0);
    HR(IDirectInputDevice8_EnumObjects(device,axes,NULL,DIDFT_AXIS));
    CHECK(z_axis==1 && rz_axis==1);
    {
        const SHORT right[][2]={{-32768,0},{32767,0},{0,32767},{0,-32768},{0,0}};
        const LONG expected[][2]={{-2000,0},{2000,0},{0,-2000},{0,2000},{0,0}};
        for(unsigned i=0;i<5;i++) {
            pad.rx=right[i][0];pad.ry=right[i][1];publish(&pad);
            CHECK(getstate(0,&xs)==0 && xs.Gamepad.sThumbRX==pad.rx && xs.Gamepad.sThumbRY==pad.ry);
            HR(IDirectInputDevice8_GetDeviceState(device,sizeof(state),&state));
            CHECK(state.lZ==expected[i][0] && state.lRz==expected[i][1]);
            CHECK(state.lX==0 && state.lY==0 && !state.rgbButtons[0] && !state.rgbButtons[1]);
            printf("PASS shipped XInput -> shipped DirectInput Z/Rz vector %u: %ld,%ld\n",i,state.lZ,state.lRz);
        }
    }

    pad.lx=-32768; pad.ly=32767; pad.rx=32767; pad.ry=-32768; pad.lt=128; pad.rt=64; pad.buttons=XINPUT_GAMEPAD_A;
    publish(&pad);
    CHECK(getstate(0,&xs)==0 && xs.Gamepad.sThumbLX==pad.lx && xs.Gamepad.bLeftTrigger==128);
    HR(IDirectInputDevice8_GetDeviceState(device,sizeof(state),&state));
    CHECK(state.lX==-2000 && state.lY==-2000 && state.lZ==2000 && state.lRz==2000);
    CHECK(state.rgbButtons[1]==0x80 && state.rgbButtons[0]==0);
    CHECK(state.lRx>0 && state.lRx<20 && state.lRy>-1020 && state.lRy<-980);
    pad.lx=16384; pad.ly=0; pad.rx=0; pad.ry=0; pad.buttons=0; publish(&pad);
    HR(IDirectInputDevice8_GetDeviceState(device,sizeof(state),&state));
    CHECK(state.lX>=999 && state.lX<=1001 && !state.lY && !state.lZ && !state.lRz && !state.rgbButtons[1]);
    {
        static const WORD masks[16]={XINPUT_GAMEPAD_B,XINPUT_GAMEPAD_A,XINPUT_GAMEPAD_X,XINPUT_GAMEPAD_Y,0,0,
          XINPUT_GAMEPAD_LEFT_SHOULDER,XINPUT_GAMEPAD_RIGHT_SHOULDER,XINPUT_GAMEPAD_BACK,XINPUT_GAMEPAD_LEFT_THUMB,
          XINPUT_GAMEPAD_RIGHT_THUMB,XINPUT_GAMEPAD_START,XINPUT_GAMEPAD_DPAD_UP,XINPUT_GAMEPAD_DPAD_RIGHT,
          XINPUT_GAMEPAD_DPAD_DOWN,XINPUT_GAMEPAD_DPAD_LEFT};
        unsigned i,j;
        for(i=0;i<16;++i) {
            memset(&pad,0,sizeof(pad)); pad.connected=1; pad.buttons=masks[i];
            pad.lt=i==4?255:0; pad.rt=i==5?255:0; publish(&pad);
            HR(IDirectInputDevice8_GetDeviceState(device,sizeof(state),&state));
            for(j=0;j<16;++j) CHECK(state.rgbButtons[j]==(i==j?0x80:0));
            CHECK(state.lX==0 && state.lY==0 && state.lZ==0 && state.lRz==0);
            CHECK(state.rgdwPOV[0]==(ps2 || i<12 ? (DWORD)-1 : (i-12)*9000));
        }
    }
    {
        char record[128]={0}; DWORD count; HANDLE f=CreateFileA("C:\\pw-pad-r2.health",GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
        CHECK(f!=INVALID_HANDLE_VALUE); CHECK(ReadFile(f,record,sizeof(record)-1,&count,NULL)); CloseHandle(f);
        CHECK(strstr(record,"PWDI2 host-r2-test ")==record);
    }
    pad.connected=0; publish(&pad);
    CHECK(IDirectInputDevice8_GetDeviceState(device,sizeof(state),&state)==DIERR_INPUTLOST);
    memset(&pad,0,sizeof(pad)); pad.connected=1; publish(&pad);
    HR(IDirectInputDevice8_Acquire(device)); HR(IDirectInputDevice8_GetDeviceState(device,sizeof(state),&state));
    HR(IDirectInputDevice8_Unacquire(device)); IDirectInputDevice8_Release(device); IDirectInput8_Release(input);
    puts("PASS R4 PE32 WoW64: POV isolation and PS2 button order; real shipped XInput PE -> real Unix library -> title sink ABI -> DirectInput, late connect, axes/buttons, heartbeat, reconnect");
    return 0;
}
