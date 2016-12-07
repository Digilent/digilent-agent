function Component() {

    //Register processes that should be stopped before this component is added/updated/removed
    component.addStopProcessForUpdateRequest("earlgrey.exe");

    var programFiles = installer.environmentVariable("ProgramFiles");
    if (programFiles != "") {
        installer.setValue("TargetDir", programFiles + "/Digilent/WaveForms Live Agent");
    }
    installer.finishButtonClicked.connect(this, Component.prototype.installationFinished);


}

Component.prototype.installationFinished = function () {
    /*
    try {
        //Windows Post Install Actions
        if (systemInfo.productType === "windows") {
            if (installer.isInstaller() && installer.status == QInstaller.Success) {
                //Launch agent now
                QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/earlgrey.exe");
            }
        }
    } catch (e) {
        console.log(e);
    }
    */
}


//Auto start on system boot
Component.prototype.createOperations = function () {
    component.createOperations();

    ////Windows Install Actions
    if (systemInfo.productType === "windows") {
        var exePath = installer.value("TargetDir") + "/earlgrey.exe";
        component.addOperation("CreateShortcut", exePath, "@UserStartMenuProgramsPath@/Startup/WaveForms-Live-Agent.lnk", "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/system32/SHELL32.dll", "iconId=2");
    }
}