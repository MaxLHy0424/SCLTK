namespace CRCSN{
    namespace Wnd{
        void Init(bool controls,bool alpha){
            std::string strTitle{(IsUserAnAdmin())?("[Admin]"):("[User]")};
            strTitle+=" CRCSN";
            SetConsoleTitle(strTitle.c_str());
            if(controls==false){
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
            system("taskKill /f /im StudentMain.exe");
            system("taskKill /f /im DispcapHelper.exe");
            system("taskKill /f /im InstHelpApp.exe");
            system("taskKill /f /im GATESRV.exe");
            system("taskKill /f /im ProcHelper64.exe");
            system("taskKill /f /im MasterHelper.exe");
            if(IsUserAnAdmin()){
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\StudentMain.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DispcapHelper.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\VRCwPlayer.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp64.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\TDOvrSet.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcHelper64.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\" /f /t reg_sz /v debugger /d ?");
                system("net stop STUDSRV /y");
                system("net stop TDNetFilter /y");
                system("net stop TDFileFilter /y");
            }
            system("cls");
            return false;
        }
        bool LenovoEClass(Parameter){
            system("taskKill /f /im vncviewer.exe");
            system("taskKill /f /im tvnserver32.exe");
            system("taskKill /f /im WfbsPnpInstall.exe");
            system("taskKill /f /im WFBSMon.exe");
            system("taskKill /f /im WFBSMlogon.exe");
            system("taskKill /f /im WFBSSvrLogShow.exe");
            system("taskKill /f /im ResetIp.exe");
            system("taskKill /f /im Fireware.exe");
            system("taskKill /f /im BCDBootCopy.exe");
            system("taskKill /f /im refreship.exe");
            system("taskKill /f /im LenovoLockScreen.exe");
            system("taskKill /f /im PortControl64.exe");
            system("taskKill /f /im DesktopCheck.exe");
            system("taskKill /f /im DeploymentManager.exe");
            system("taskKill /f /im DeploymentAgent.exe");
            system("taskKill /f /im XYNTService.exe");
            if(IsUserAnAdmin()){
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\vncviewer.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\tvnserver32.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WfbsPnpInstall.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMon.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMlogon.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSSvrLogShow.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ResetIp.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\FuncForWIN64.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\CertMgr.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Fireware.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\BCDBootCopy.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\refreship.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\LenovoLockScreen.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\PortControl64.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DesktopCheck.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentManager.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentAgent.exe\" /f /t reg_sz /v debugger /d ?");
                system("reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\XYNTService.exe\" /f /t reg_sz /v debugger /d ?");
                system("net stop BSAgentSvr /y");
                system("net stop tvnserver /y");
                system("net stop WFBSMlogon /y");
            }
            system("cls");
            return false;
        }
    }
    namespace Recovery{
        bool ELearningClass(Parameter){
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\StudentMain.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DispcapHelper.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\VRCwPlayer.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\InstHelpApp64.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\TDOvrSet.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\GATESRV.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcHelper64.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MasterHelper.exe\" /f");
            system("net start STUDSRV");
            system("net start TDNetFilter");
            system("net start TDFileFilter");
            system("cls");
            return false;
        }
        bool LenovoEClass(Parameter){
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\vncviewer.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\tvnserver32.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WfbsPnpInstall.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMon.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSMlogon.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\WFBSSvrLogShow.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ResetIp.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\FuncForWIN64.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\CertMgr.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Fireware.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\BCDBootCopy.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\refreship.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\LenovoLockScreen.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\PortControl64.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DesktopCheck.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentManager.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\DeploymentAgent.exe\" /f");
            system("reg delete \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\XYNTService.exe\" /f");
            system("net start BSAgentSvr");
            system("net start tvnserver");
            system("net start WFBSMlogon");
            system("cls");
            return false;
        }
    }
}