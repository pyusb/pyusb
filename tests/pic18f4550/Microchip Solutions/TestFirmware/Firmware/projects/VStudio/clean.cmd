@ECHO OFF
.\Utility\RegexClean -m"\.(ncb|resharper|suo|user|err|mcs|mptags|tagsrc)$"
.\Utility\RegexClean -r -d -m"\\_ReSharper\."
.\Utility\RegexClean -r -d -m"\\(Release)$"
