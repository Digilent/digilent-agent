function Component() {

    //Register processes that should be stopped before this component is added/updated/removed
    component.addStopProcessForUpdateRequest("waveforms-live-agent.exe");

    var programFiles = installer.environmentVariable("ProgramFiles");
    if (programFiles != "") {
        installer.setValue("TargetDir", programFiles + "/Digilent/WaveForms Live Agent");
    }
}

//Platform Specific Custom Actions
Component.prototype.createOperations = function () {
    //Call default createOperations
    component.createOperations();

    ////Windows Install Actions
    if (systemInfo.productType === "windows") {
        //Create Shortcut To Start At System Boot
        var exePath = installer.value("TargetDir") + "/waveforms-live-agent.exe";
        component.addOperation("CreateShortcut", exePath, "@UserStartMenuProgramsPath@/Startup/WaveForms-Live-Agent.lnk", "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/system32/SHELL32.dll", "iconId=2");

        //Create start menu shortcut
        component.addOperation("CreateShortcut", "@TargetDir@/waveforms-live-agent.exe", "@StartMenuDir@/WaveForms Live Agent.lnk");
    }


    //Create Applications link on OSX
    //if (systemInfo.productType === "macos") {
    var appPath = installer.value("TargetDir") + "/waveforms-live-agent.app/";
        component.addOperation("Execute", "sudo ln -s /Library/Digilent/WaveFormsLiveAgent/waveforms-live-agent.app /Applications/");
    //}



}
