struct RolloutBuilderStruct (
private

    kRoot     = Dictionary #string,
    kDialog   = undefined,
    kTitle    = "Main Window",
    kLayout   = "grid",
    kSize     = Point2 640 480,
    kControls = #(),

    kBuildGlobalPrefix = "rbg_",
    kBuildRolloutName  = "",

public

    fn SetLayoutFromFile filename = (
        this.kRoot     = readJson filename
        this.kDialog   = this.kRoot["dialog"]
        this.kTitle    = this.kRoot["dialog"]["properties"]["title"]
        this.kLayout   = this.kRoot["dialog"]["layout"]
        this.kSize     = Point2 this.kDialog["geometry"]["width"] this.kDialog["geometry"]["height"]
        this.kControls = this.kRoot["dialog"]["controls"]

        this.kBuildRolloutName = this.kBuildGlobalPrefix + (SubstituteString this.kTitle " " "")
    ),

    fn Build = (
        local buildStr = StringStream ""

        format "global %\n" this.kBuildRolloutName to:buildStr
        format "rollout % \"%\" width:% height:% (\n" this.kBuildRolloutName this.kTitle this.kSize[1] this.kSize[2] to:buildStr
        format "button myButton \"myButton\"\n" to:buildStr
        format ")" to:buildStr

        execute (buildStr as string)
    ),

    fn Show = (
        local showStr = StringStream ""

        format "DestroyDialog %\n" this.kBuildRolloutName to:showStr
        format "CreateDialog % " this.kBuildRolloutName to:showStr

        this.Close()
        execute (showStr as string)
    ),

    fn Close = (
        local closeStr = StringStream ""

        format "DestroyDialog %" this.kBuildRolloutName to:closeStr

        execute (closeStr as string)
    )
)

-- Example
::rb = RolloutBuilderStruct()
rb.SetLayoutFromFile @"..\ui\basicdialog.json"
rb.Build()
rb.Show()
