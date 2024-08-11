namespace CRCSN{
    struct Opt{
        bool ctrls,frontShow,alphaWnd;
    }Opt{};
    struct Rule{
        struct Mythware{
            std::string exe[9],svc[3];
        }Mythware;
        struct Lenovo{
            std::string exe[18],svc[3];
        }Lenovo;
    }Rule{
        {
            {
                "StudentMain","DispcapHelper","VRCwPlayer",
                "InstHelpApp","InstHelpApp64","TDOvrSet",
                "GATESRV","ProcHelper64","MasterHelper"
            },{
                "STUDSRV","TDNetFilter","TDFileFilter"
            }
        },{
            {
                "vncviewer","tvnserver32","WfbsPnpInstall",
                "WFBSMon","WFBSMlogon","WFBSSvrLogShow",
                "ResetIp","FuncForWIN64","CertMgr",
                "Fireware","BCDBootCopy","refreship",
                "LenovoLockScreen","PortControl64","DesktopCheck",
                "DeploymentManager","DeploymentAgent","XYNTService"
            },{
                "BSAgentSvr","tvnserver","WFBSMlogon"
            }
        }
    };
    auto Init(bool ctrls,bool alpha){
        system("chcp 936 > nul");
        #if EVALUATE_COPY
            SetConsoleTitle("[评估副本] CRCSN");
        #else
            SetConsoleTitle("CRCSN");
        #endif
        if(!ctrls){
            SetWindowLongPtr(GetConsoleWindow(),GWL_STYLE,GetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX);
        }
        system("mode con cols=50 lines=25");
        SetLayeredWindowAttributes(GetForegroundWindow(),0ul,((alpha)?(230u):(255u)),LWA_ALPHA);
    }
    auto FrontShow(){
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
            Sleep(100ul);
        }
    }
    auto Cmd(Parameter){
        system("cls & cmd & cls");
        Init(Opt.ctrls,Opt.alphaWnd);
        return false;
    }
    auto OpBase(char mode,std::string* exe,uint16_t n,std::string* svc,uint16_t m){
        system("cls");
        std::string cmd;
        switch(mode){
            case 'c':{
                for(uint16_t i{};i<n;++i){
                    cmd="taskKill /f /im "+exe[i]+".exe";
                    system(cmd.c_str());
                }
                for(uint16_t i{};i<n;++i){
                    cmd="reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\"+exe[i]+".exe\" /f /t reg_sz /v debugger /d ?";
                    system(cmd.c_str());
                }
                for(uint16_t i{};i<m;++i){
                    cmd="net stop "+svc[i]+" /y";
                    system(cmd.c_str());
                }
                break;
            }case 'r':{
                for(uint16_t i{};i<n;++i){
                    cmd="reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\"+exe[i]+".exe\" /f";
                    system(cmd.c_str());
                }
                for(uint16_t i{};i<m;++i){
                    cmd="net start "+svc[i];
                    system(cmd.c_str());
                }
                break;
            }
        }
        system("cls");
    }
    namespace Crack{
        auto Mythware(Parameter){
            OpBase('c',Rule.Mythware.exe,9u,Rule.Mythware.svc,3u);
            return false;
        }
        auto Lenovo(Parameter){
            OpBase('c',Rule.Lenovo.exe,18u,Rule.Lenovo.svc,3u);
            return false;
        }
    }
    namespace Recovery{
        auto Mythware(Parameter){
            OpBase('r',Rule.Mythware.exe,9u,Rule.Mythware.svc,3u);
            return false;
        }
        auto Lenovo(Parameter){
            OpBase('r',Rule.Lenovo.exe,18u,Rule.Lenovo.svc,3u);
            return false;
        }
    }
}