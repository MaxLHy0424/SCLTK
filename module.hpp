namespace CRCSN{
    struct Opt{
        bool ctrls,frontShow,alphaWnd;
    }Opt;
    struct Rule{
        struct Mythware{
            std::string exe[9],svc[3];
        }Mythware{
            {
                "StudentMain","DispcapHelper","VRCwPlayer",
                "InstHelpApp","InstHelpApp64","TDOvrSet",
                "GATESRV","ProcHelper64","MasterHelper"
            },{
                "STUDSRV","TDNetFilter","TDFileFilter"
            }
        };
        struct Lenovo{
            std::string exe[18],svc[3];
        }Lenovo{
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
        };
    }Rule;
    void Init(bool ctrls,bool alpha){
        system("chcp 936");
        #if EvaluateCopy
            SetConsoleTitle("[评估副本] CRCSN");
        #else
            SetConsoleTitle("CRCSN");
        #endif
        if(!ctrls){
            SetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE,GetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX);
        }
        system("mode con cols=50 lines=25");
        SetLayeredWindowAttributes(GetForegroundWindow(),0,((alpha)?(204):(255)),LWA_ALPHA);
        return;
    }
    void FrontShow(){
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
    namespace Crack{
        void Core(std::string* exe,uint16_t n,std::string* svc,uint16_t m){
            system("cls");
            std::string cmd;
            for(uint16_t i{};i<n;++i){
                cmd="taskKill /f /im "+exe[i]+".exe";
                system(cmd.c_str());
            }
            if(IsUserAnAdmin()){
                for(uint16_t i{};i<n;++i){
                    cmd="reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\"+exe[i]+".exe\" /f /t reg_sz /v debugger /d ?";
                    system(cmd.c_str());
                }
                for(uint16_t i{};i<m;++i){
                    cmd="net stop "+svc[i]+" /y";
                    system(cmd.c_str());
                }
            }
            system("cls");
            return;
        }
        bool Mythware(Parameter){
            Core(Rule.Mythware.exe,9,Rule.Mythware.svc,3);
            return false;
        }
        bool Lenovo(Parameter){
            Core(Rule.Lenovo.exe,18,Rule.Lenovo.svc,3);
            return false;
        }
    };
    namespace Recovery{
        void Core(std::string* exe,uint16_t n,std::string* svc,uint16_t m){
            system("cls");
            std::string cmd;
            for(uint16_t i{};i<n;++i){
                cmd="reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\"+exe[i]+".exe\" /f";
                system(cmd.c_str());
            }
            for(uint16_t i{};i<m;++i){
                cmd="net start "+svc[i];
                system(cmd.c_str());
            }
            system("cls");
            return;
        }
        bool Mythware(Parameter){
            Core(Rule.Mythware.exe,9U,Rule.Mythware.svc,3U);
            return false;
        }
        bool Lenovo(Parameter){
            Core(Rule.Lenovo.exe,18U,Rule.Lenovo.svc,3U);
            return false;
        }
    }
    bool Cmd(Parameter){
        system("cls & cd %UserProfile% & cmd");
        Init(Opt.ctrls,Opt.alphaWnd);
        return false;
    }
}