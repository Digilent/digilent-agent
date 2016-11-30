function Component()
{
     var programFiles = installer.environmentVariable("ProgramFiles");
     if (programFiles != "")
         installer.setValue("TargetDir", programFiles + "/Digilent/WaveForms Live Agent");
}