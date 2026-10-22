/* SPDX-License-Identifier: LGPL-2.1-or-later */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#include "ps2_settings.h"
static void report(const char *message,int quiet) {
    FILE *f=fopen("C:\\pw-ps2-setup.txt","wb");
    if(f) { fprintf(f,"PW_PS2_SETUP R4 %s\n",message); fclose(f); }
    fprintf(stderr,"PW_PS2_SETUP R4 %s\n",message);
    OutputDebugStringA(message);
    if(!quiet) MessageBoxA(NULL,message,"GTA SA R4 - configuration PS2",MB_OK);
}
int main(int argc,char **argv) {
    WCHAR path[1024],backup[1100],temp[1100];
    int quiet=0,restore=0,invert=1,camera_only=0; const char *explicit_path=NULL;
    for(int i=1;i<argc;i++) {
        if(!strcmp(argv[i],"--quiet")) quiet=1;
        else if(!strcmp(argv[i],"--restore")) restore=1;
        else if(!strcmp(argv[i],"--invert-right-y")) { invert=1; camera_only=1; }
        else if(!strcmp(argv[i],"--right-y-original")) { invert=0; camera_only=1; }
        else if(!strcmp(argv[i],"--file") && i+1<argc) explicit_path=argv[++i];
        else return 10;
    }
    if(explicit_path) {
        if(!MultiByteToWideChar(CP_UTF8,0,explicit_path,-1,path,1024)) return 11;
    } else {
        if(FAILED(SHGetFolderPathW(NULL,CSIDL_PERSONAL,NULL,SHGFP_TYPE_CURRENT,path))) {
            report("Documents introuvable. Aucun fichier modifie.",quiet); return 12;
        }
        if(wcslen(path)>800) return 13;
        wcscat(path,L"\\GTA San Andreas User Files\\gta_sa.set");
    }
    _snwprintf(backup,1100,L"%ls.pw-before-ps2-r4",path);
    _snwprintf(temp,1100,L"%ls.pw-ps2-r4.tmp",path);
    if(restore) {
        if(!CopyFileW(backup,temp,FALSE) ||
           !MoveFileExW(temp,path,MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)) {
            report("Restauration impossible. Verifier la sauvegarde .pw-before-ps2-r4.",quiet); return 14;
        }
        report("Configuration avant R4 restauree. Relancer le profil de secours R3.",quiet); return 0;
    }
    unsigned char bytes[PW_PS2_SIZE+1]; size_t size;
    FILE *f=_wfopen(path,L"rb");
    if(!f) {
        report("gta_sa.set absent. Lancer GTA avec le profil R3 conserve, puis quitter normalement. Relancer ensuite cet outil. Aucun fichier modifie.",quiet); return 15;
    }
    size=fread(bytes,1,sizeof(bytes),f); int read_error=ferror(f); fclose(f);
    if(read_error || !pw_ps2_validate(bytes,size)) {
        report("Format de gta_sa.set non reconnu (v6/v7, 2900 octets). Aucun fichier modifie. Utiliser les reglages Joypad de GTA et recuperer le fichier pour analyse.",quiet); return 16;
    }
    if(camera_only && pw_ps2_get(bytes)==7) {
        report("Version 7 : utiliser Joypad Settings dans GTA pour le sens Y du stick droit. Les options v7 sont preservees sans supposer leurs offsets. Aucun fichier modifie.",quiet); return 20;
    }
    if(camera_only) bytes[PW_PS2_TAIL+51]=(unsigned char)invert;
    else if(!pw_ps2_apply(bytes,size,invert)) return 16;
    if(!CopyFileW(path,backup,TRUE) && GetLastError()!=ERROR_FILE_EXISTS) {
        report("Sauvegarde impossible. Aucun reglage remplace.",quiet); return 17;
    }
    f=_wfopen(temp,L"wb");
    if(!f) return 18;
    int wrote=fwrite(bytes,1,size,f)==size;
    int flushed=fflush(f)==0,closed=fclose(f)==0;
    if(!wrote || !flushed || !closed ||
       !MoveFileExW(temp,path,MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)) {
        report("Ecriture impossible. Conserver la sauvegarde et recuperer le journal.",quiet); return 19;
    }
    if(pw_ps2_get(bytes)==7) report("Affectations PS2 v7 enregistrees. Dans GTA : Options > Controller Setup > Configuration = Joypad. Regler le sens du stick droit dans Joypad Settings. Puis maintenir R3 et cliquer le tactile. Toutes les options v7 (graphismes, audio, camera) sont preservees.",quiet);
    else if(camera_only) report("Seul le sens Y du joystick droit a ete configure. Relancer GTA et verifier la camera.",quiet);
    else report("Configuration PS2 enregistree : Joypad, actions par contexte, axes sans permutation, Y droit inverse pour visee directe. Graphismes, audio et clavier preserves. Lancer GTA SA R4 - PS2 natif, puis maintenir R3 et cliquer le tactile pour activer les commandes natives. Tactile seul = secours R3.",quiet);
    return 0;
}
