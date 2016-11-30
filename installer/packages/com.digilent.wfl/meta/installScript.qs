function Component() {
    var programFiles = installer.environmentVariable("ProgramFiles");
    if (programFiles != "") {
        installer.setValue("TargetDir", programFiles + "/Digilent/WaveForms Live Agent");
    }
    installer.finishButtonClicked.connect(this, Component.prototype.installationFinished);
}

Component.prototype.installationFinished = function () {
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/earlgrey.exe");
        }
    } catch (e) {
        console.log(e);
    }

}
