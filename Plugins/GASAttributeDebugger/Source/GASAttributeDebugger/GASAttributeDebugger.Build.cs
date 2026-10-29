using UnrealBuildTool;

public class GASAttributeDebugger : ModuleRules
{
	public GASAttributeDebugger(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"GameplayAbilities",
		});
	}
}
