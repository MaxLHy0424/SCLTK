namespace Pvt{
    struct{
        bool wndCtrls,frontShow,alphaWnd,sparseView;
    }opt{};
    struct{
        struct{
            std::string exe[9],svc[3];
        }mythware;
        struct{
            std::string exe[18],svc[3];
        }lenovo;
    }rule{
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
                "lenovoLockScreen","PortControl64","DesktopCheck",
                "DeploymentManager","DeploymentAgent","XYNTService"
            },{
                "BSAgentSvr","tvnserver","WFBSMlogon"
            }
        }
    };
    auto init(bool wndCtrls,bool alpha){
        system("chcp 936 > nul");
        #if EVALUATE_COPY
            SetConsoleTitle("[评估副本] CRCSN");
        #else
            SetConsoleTitle("CRCSN");
        #endif
        if(!wndCtrls){
            SetWindowLongPtr(GetConsoleWindow(),GWL_STYLE,GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX);
        }
        system("mode con cols=50 lines=25");
        SetLayeredWindowAttributes(GetForegroundWindow(),0ul,((alpha)?(230u):(255u)),LWA_ALPHA);
    }
    auto frontShow(){
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
    auto cmd(Parameter){
        system("cls & cmd & cls");
        init(opt.wndCtrls,opt.alphaWnd);
        return false;
    }
    auto opBase(char mode,std::string* exe,uint16_t n,std::string* svc,uint16_t m){
        system("cls");
        std::string cmd;
        switch(mode){
            case 'C':{
                for(uint16_t i{};i<n;++i){
                    cmd="taskKill /f /im "+exe[i]+".exe";
                    system(cmd.c_str());
                }
                for(uint16_t i{};i<n;++i){
                    cmd="reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"+exe[i]+".exe\" /f /t reg_sz /v debugger /d ?";
                    system(cmd.c_str());
                }
                for(uint16_t i{};i<m;++i){
                    cmd="net stop "+svc[i]+" /y";
                    system(cmd.c_str());
                }
                break;
            }case 'R':{
                for(uint16_t i{};i<n;++i){
                    cmd="reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"+exe[i]+".exe\" /f";
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
        auto mythware(Parameter){
            opBase('C',rule.mythware.exe,9u,rule.mythware.svc,3u);
            return false;
        }
        auto lenovo(Parameter){
            opBase('C',rule.lenovo.exe,18u,rule.lenovo.svc,3u);
            return false;
        }
    }
    namespace Recovery{
        auto mythware(Parameter){
            opBase('R',rule.mythware.exe,9u,rule.mythware.svc,3u);
            return false;
        }
        auto lenovo(Parameter){
            opBase('R',rule.lenovo.exe,18u,rule.lenovo.svc,3u);
            return false;
        }
    }
}