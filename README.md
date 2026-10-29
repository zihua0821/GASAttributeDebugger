# GAS Attribute Debugger

在编辑器里实时查看和修改任意 Actor 的 GAS 属性，不用重启 PIE。

> UE 5.5+ · 仅编辑器 · 需要 GameplayAbilities

## 为什么做这个

调 GAS 时一轮验证大概是：改一个值 → 重启 PIE → 复现 → 看数值 → 再改。一轮 30 秒，一天能重复上百次。

引擎自带的 `showdebug abilitysystem` 能看当前世界的 ASC，但有两个限制：

- 得自己敲控制台命令，还要提前知道属性名
- 多开 PIE 时只显示当前窗口，分不清「我改的是服务器还是某个客户端」

这个面板就是解决这两点：

- 世界 → Actor → AttributeSet → 属性，逐级点选，不用记名字
- 世界下拉里每个世界都带 NetMode，多开 PIE 不会改错端
- 改完立即生效，行上的数值实时回读

## 功能

- 多开 PIE 时自动列出所有世界，标明 NetMode（`ListenServer 0` / `Client 1` …）
- 联动选择：世界 → Actor（实现 `AbilitySystemInterface` 的）→ AttributeSet → 属性
- 运行时临时造 Instant GE 施加修正
- 走真实 GAS 管线，不是直接 Set 属性：属性变化委托、Clamp 都会正常触发
- 属性变化实时回读
- 标出属性是否 Replicated（反射查 `CPF_Net`），避免「为什么客户端血条不动」的误判

## 安装

1. 把 `Plugins/GASAttributeDebugger` 整个目录复制到你项目的 `Plugins/` 下
2. 确认你项目的 `.uproject` 启用了 `GameplayAbilities`（必需）和 `EditorScriptingUtilities`（面板依赖）
3. 重新生成工程文件、编译、重启编辑器

插件自带内容，两个 Editor Utility Widget 就在插件里，装完即可用：

```
/GASAttributeDebugger/Tools/AttributesEditor
/GASAttributeDebugger/Tools/AttributeLine
```

想快速试：直接打开仓库根目录的 `GASAttributeDebugger.uproject`，编辑器会提示重建缺失模块，点「是」。注意这是个空工程，里面没有任何 GAS 内容，Actor 下拉会是空的——要验证实际效果，请把插件装进你自己的（有 GAS 的）项目。

## 打开面板

面板不会自动出现在任何菜单里，**第一次需要手动打开**：

1. Content Browser → 右下角 **Settings** → 勾 **Show Plugin Content**
2. 左侧目录树进入 **Plugins → GASAttributeDebugger → Tools**
3. 双击 `AttributesEditor`（或右键 → **Run Editor Utility Widget**）

打开后是一个可停靠的标签页，可以拖到任意位置。

### 为什么「工具」菜单里一开始没有它

`工具 → Editor Utility Widgets` 这个子菜单不是扫描资产生成的，它列的是「已注册的 Editor Utility Widget 标签页」，来源是用户配置里的 `LoadedUIs` 数组：

```
Saved/Config/WindowsEditor/EditorPerProjectUserSettings.ini
```

只有你打开过一次面板，它才会被写进这个数组。所以：

- **第一次**：从内容浏览器打开（上面 3 步）
- **之后**：它会出现在 `工具 → Editor Utility Widgets` 里，可直接点

这是引擎的机制，不是这个插件的限制。

## 依赖与版本

| 项 | 说明 |
|---|---|
| 引擎 | **UE 5.5+** |
| GameplayAbilities | 必需 |
| Editor Scripting Utilities | 编辑期需要 |
| 模块类型 | `Editor`，不会打进包里 |

### 为什么是 5.5+

两个 Widget 是以 5.5 保存的（`FileVersionUE5 = 1013`）。引擎对「包版本高于当前引擎」的包是硬失败——`LinkerLoad.cpp` 里报 `Package ... contains a newer version than the current process supports` 之后直接 `return LINKER_Failed`，没有降级加载的开关：

| 引擎 | 支持的最大包版本 | 能否加载 |
|---|---|---|
| 5.3 | 1009 | 否 |
| 5.4 | 1012 | 否 |
| 5.5 | 1013 | 是 |

C++ 那 4 个文件没用到任何 5.5 专有 API，5.0~5.5 都能编。**卡住的只是资产**：要给 5.3/5.4 用，得在对应版本里重搭这两个 Widget，C++ 不用改。

## 设计说明

C++ 只做三件蓝图做不了的事：

1. 用 GAS API 枚举 AttributeSet / 属性 / 读数值
2. 运行时 `NewObject<UGameplayEffect>` 临时造 Instant GE（蓝图运行时造不出来）
3. 反射查 `CPF_Net` 判断属性是否复制

其余全在蓝图：世界 / Actor / Set 联动、属性行动态生成、输入校验、委托回流刷新。改下拉项、调布局都不需要重新编译。

## 已知限制

- 只对 Self 施加，不能对目标施加
- 只在当前世界生效：在 Client 窗口里改，不会同步回服务器
- 只造 Instant GE，不产生持续效果
- 判复制只看 `CPF_Net`（`UPROPERTY(Replicated)`），对「整个 AttributeSet 用条件复制」判断不了
- `UAttributeLineHandle` 用 `AddToRoot` 保活，必须成对调用 `Destroy()`，否则对象会挂在 GC Root 上
- 下拉里的 9 个 ModOp 实际只有 6 种行为：`Additive (Old)` / `Multiplicitive (Old)` / `Division (Old)` 是引擎的向后兼容别名，分别等价于 `AddBase` / `MultiplyAdditive` / `DivideAdditive`

## 同类工具

GAS 调试这个方向不是空的，Cog、GASAttachEditor 这类工具都做过类似的事。这个面板只是把「多开 PIE 分不清改的是哪个端」这个问题解决得比较直接。

## License

MIT，见 [LICENSE](LICENSE)。
