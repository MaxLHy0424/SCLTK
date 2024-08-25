namespace Mod{
    struct{
        bool wndSize,frontShow,alphaWnd,sparseView;
    }opt{};
    auto init(bool wndSize,bool alpha){
        system("chcp 936 > nul");
        #if CHANNEL==0
            SetConsoleTitle("CRCSN");
        #elif CHANNEL==1
            SetConsoleTitle("[Beta] CRCSN");
        #elif CHANNEL==2
            SetConsoleTitle("[Dev] CRCSN");
        #else
            SetConsoleTitle("[CUSTOM] CRCSN");
        #endif
        SetWindowLongPtr(GetConsoleWindow(),GWL_STYLE,(wndSize)?\
            (GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)|WS_SIZEBOX|WS_MAXIMIZEBOX):\
            (GetWindowLongPtr(GetConsoleWindow(),GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX)
        );
        system("mode con cols=50 lines=25");
        SetLayeredWindowAttributes(GetForegroundWindow(),0,((alpha)?(230):(255)),LWA_ALPHA);
    }
    auto frontShow(){
        HWND foreWnd{GetConsoleWindow()};
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
        init(opt.wndSize,opt.alphaWnd);
        return false;
    }
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
    auto opBase(char op,std::string* exe,u16 n,std::string* svc,u16 m){
        system("cls");
        std::string cmd;
        switch(op){
            case 'C':{
                for(u16 i{};i<n;++i){
                    cmd="taskKill /f /im "+exe[i]+".exe";
                    system(cmd.c_str());
                }
                for(u16 i{};i<n;++i){
                    cmd="reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"+exe[i]+".exe\" /f /t reg_sz /v debugger /d ?";
                    system(cmd.c_str());
                }
                for(u16 i{};i<m;++i){
                    cmd="net stop "+svc[i]+" /y";
                    system(cmd.c_str());
                }
                break;
            }case 'R':{
                for(u16 i{};i<n;++i){
                    cmd="reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution options\\"+exe[i]+".exe\" /f";
                    system(cmd.c_str());
                }
                for(u16 i{};i<m;++i){
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
            opBase('C',rule.mythware.exe,9,rule.mythware.svc,3);
            return false;
        }
        auto lenovo(Parameter){
            opBase('C',rule.lenovo.exe,18,rule.lenovo.svc,3);
            return false;
        }
    }
    namespace Recovery{
        auto mythware(Parameter){
            opBase('R',rule.mythware.exe,9,rule.mythware.svc,3);
            return false;
        }
        auto lenovo(Parameter){
            opBase('R',rule.lenovo.exe,18,rule.lenovo.svc,3);
            return false;
        }
    }
}