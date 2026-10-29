using UnrealBuildTool;

public class GASAttributeDebuggerTarget : TargetRules
{
	public GASAttributeDebuggerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("GASAttributeDebuggerProject");
	}
}