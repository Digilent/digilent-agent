function Component() {

    //Register processes that should be stopped before this component is added/updated/removed
    component.addStopProcessForUpdateRequest("waveforms-live-agent.exe");
    component.addStopProcessForUpdateRequest("digilent-agent.exe");

    var programFiles = installer.environmentVariable("ProgramFiles");
    if (programFiles != "") {
        installer.setValue("TargetDir", programFiles + "/Digilent/Agent");
    }
}

//Platform Specific Custom Actions
Component.prototype.createOperations = function () {
    //Call default createOperations
    component.createOperations();

    ////Windows Install Actions
    if (systemInfo.productType === "windows") {
        //Create Shortcut To Start At System Boot
        var exePath = installer.value("TargetDir") + "/digilent-agent.exe";
        component.addOperation("CreateShortcut", exePath, "@UserStartMenuProgramsPath@/Startup/Digilent-Agent.lnk", "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/system32/SHELL32.dll", "iconId=2");

        //Create start menu shortcut
        component.addOperation("CreateShortcut", "@TargetDir@/digilent-agent.exe", "@StartMenuDir@/Digilent Agent.lnk");
    }

}
