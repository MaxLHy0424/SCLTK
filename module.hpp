namespace CRCSN{
    namespace Wnd{
        void Init(bool ctrls,bool alpha){
            std::string title{(IsUserAnAdmin())?("Admin"):("User")};
            title="["+title+"] CRCSN";
            SetConsoleTitle(title.c_str());
            if(!ctrls){
                SetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE,GetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX);
            }
            system("mode con cols=50 lines=30");
            SetLayeredWindowAttributes(GetForegroundWindow(),0,((alpha)?(204):(255)),LWA_ALPHA);
        }
        void Pin(){
            HWND foreWnd{GetForegroundWindow()};
            DWORD foreId{GetWindowThreadProcessId(foreWnd,NULL)},curId{GetCurrentThreadId()};
            while(true){
                AttachThreadInput(curId,foreId,TRUE);
                ShowWindow(foreWnd,SW_SHOWNORMAL);
                SetWindowPos(foreWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
                SetWindowPos(foreWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
                SetForegroundWindow(foreWnd);
                AttachThreadInput(curId,foreId,FALSE);
                SetWindowPos(foreWnd,HWND_TOPMOST,0,0,100,100,SWP_NOMOVE|SWP_NOSIZE);
                Sleep(100UL);
            }
        }
    }
    namespace Crack{
        bool ELearningClass(Parameter){
            system("cls");
            std::string exe[9]{
                "StudentMain","DispcapHelper","VRCwPlayer",
                "InstHelpApp","InstHelpApp64","TDOvrSet",
                "GATESRV","ProcHelper64","MasterHelper"
            },svc[3]{
                "STUDSRV","TDNetFilter","TDFileFilter"
            },tmp{};
            for(unsigned short i{};i<9;++i){
                tmp="taskKill /f /im "+exe[i]+".exe";
                system(tmp.c_str());
            }
            if(IsUserAnAdmin()){
                for(unsigned short i{};i<9;++i){
                    tmp="reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\"+exe[i]+".exe\" /f /t reg_sz /v debugger /d ?";
                    system(tmp.c_str());
                }
                for(unsigned short i{};i<3;++i){
                    tmp="net stop "+svc[i]+" /y";
                    system(tmp.c_str());
                }
            }
            system("cls");
            return false;
        }
        bool LenovoEClass(Parameter){
            system("cls");
            std::string exe[18]{
                "vncviewer","tvnserver32","WfbsPnpInstall",
                "WFBSMon","WFBSMlogon","WFBSSvrLogShow",
                "ResetIp","FuncForWIN64","CertMgr",
                "Fireware","BCDBootCopy","refreship",
                "LenovoLockScreen","PortControl64","DesktopCheck",
                "DeploymentManager","DeploymentAgent","XYNTService"
            },svc[3]{
                "BSAgentSvr","tvnserver","WFBSMlogon"
            },tmp{};
            for(unsigned short i{};i<18;++i){
                tmp="taskKill /f /im "+exe[i]+".exe";
                system(tmp.c_str());
            }
            if(IsUserAnAdmin()){
                for(unsigned short i{};i<18;++i){
                    tmp="reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\"+exe[i]+".exe\" /f /t reg_sz /v debugger /d ?";
                    system(tmp.c_str());
                }
                for(unsigned short i{};i<3;++i){
                    tmp="net stop "+svc[i]+" /y";
                    system(tmp.c_str());
                }
            }
            system("cls");
            return false;
        }
    }
    namespace Recovery{
        bool ELearningClass(Parameter){
            system("cls");
            std::string exe[9]{
                "StudentMain","DispcapHelper","VRCwPlayer",
                "InstHelpApp","InstHelpApp64","TDOvrSet",
                "GATESRV","ProcHelper64","MasterHelper"
            },svc[3]{
                "STUDSRV","TDNetFilter","TDFileFilter"
            },tmp{};
            for(unsigned short i{};i<9;++i){
                tmp="reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\"+exe[i]+".exe\" /f";
                system(tmp.c_str());
            }
            for(unsigned short i{};i<3;++i){
                tmp="net start "+svc[i];
                system(tmp.c_str());
            }
            system("cls");
            return false;
        }
        bool LenovoEClass(Parameter){
            system("cls");
            std::string exe[18]{
                "vncviewer","tvnserver32","WfbsPnpInstall",
                "WFBSMon","WFBSMlogon","WFBSSvrLogShow",
                "ResetIp","FuncForWIN64","CertMgr",
                "Fireware","BCDBootCopy","refreship",
                "LenovoLockScreen","PortControl64","DesktopCheck",
                "DeploymentManager","DeploymentAgent","XYNTService"
            },svc[3]{
                "BSAgentSvr","tvnserver","WFBSMlogon"
            },tmp{};
            for(unsigned short i{};i<18;++i){
                tmp="reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\"+exe[i]+".exe\" /f";
                system(tmp.c_str());
            }
            for(unsigned short i{};i<3;++i){
                tmp="net start "+svc[i];
                system(tmp.c_str());
            }
            system("cls");
            return false;
        }
    }
}