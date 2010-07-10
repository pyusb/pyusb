@ECHO OFF
.\projects\VStudio\Utility\RegexClean -m"\.(\$\$\$|bkx|cce|cod|cof|err|hex|lde|i|lde|lst|obj|o|rlf|sym|sdb|wat|mcs|mptags|tagsrc|map|elf|ncb|resharper|suo|user)$"
IF EXIST ".\Objects\" rd Objects /S /Q
.\projects\VStudio\Utility\RegexClean -r -d -m"\\_ReSharper\."
.\projects\VStudio\Utility\RegexClean -r -d -m"\\(Release|Debug|temp|output)$"
