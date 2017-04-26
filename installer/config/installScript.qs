function Controller() {
    installer.uninstallationStarted.connect(onUninstallationStarted);
}

onUninstallationStarted = function () {

}

function skipFinishPageCallback() {
     gui.clickButton(buttons.Finish);
}

Controller.prototype.TargetDirectoryPageCallback = function () {
    var widget = gui.currentPageWidget();
    widget.TargetDirectoryLineEdit.textChanged.connect(this, Controller.prototype.targetChanged);
    Controller.prototype.targetChanged(widget.TargetDirectoryLineEdit.text);
}

Controller.prototype.targetChanged = function (text) {   
    if (text != "" && installer.fileExists(text + "/maintenancetool.exe")) {
      
        //Prompt user to launch updater
        if (QMessageBox.question("OverwriteTargetDirectory", "Launch Updater?", "A previous version exists in the specified directory.  Do you want to launch the updater?", QMessageBox.Yes | QMessageBox.No) == QMessageBox.Yes) {
            //Launch Updater
            QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/update.bat");
                              
            //Skip remaining wizard pages and quit the installer
            installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
            installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
            installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
            installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
            installer.setDefaultPageVisible(QInstaller.PerformInstallation, false);
            installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);            
            gui.clickButton(buttons.CancelButton);
        }
    }

}

