function Component() {

}

Component.prototype.createOperationsForArchive = function(archive)
{
    component.addOperation("Extract", archive, "@HomeDir@//AppData/Local/Digilent/Digilent Agent/www/");
}