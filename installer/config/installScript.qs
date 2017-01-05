function Controller() {
    installer.uninstallationStarted.connect(onUninstallationStarted);
}

onUninstallationStarted = function () {

}

Controller.prototype.TargetDirectoryPageCallback = function () {
    var widget = gui.currentPageWidget();
    widget.TargetDirectoryLineEdit.textChanged.connect(this, Controller.prototype.targetChanged);
    Controller.prototype.targetChanged(widget.TargetDirectoryLineEdit.text);
}

Controller.prototype.targetChanged = function (text) {
    installer.setValue("RemoveTargetDir", true);
    if (text != "" && installer.fileExists(text + "/components.xml")) {
        if (QMessageBox.question("OverwriteTargetDirectory", "Overwrite target directory", "Do you want to overwrite previous installation?", QMessageBox.Yes | QMessageBox.No) == QMessageBox.Yes) {
            installer.setValue("RemoveTargetDir", false);
        }
    }
}
