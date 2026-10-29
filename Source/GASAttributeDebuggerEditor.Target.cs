using UnrealBuildTool;

public class GASAttributeDebuggerEditorTarget : TargetRules
{
	public GASAttributeDebuggerEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("GASAttributeDebuggerProject");
	}
}