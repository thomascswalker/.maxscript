struct DialogResizerStruct (
public

    /*
     * 
     */
    fn GetControlPos &control = (
        return Point2 control.pos[1] control.pos[2]
    ),

    /*
     * 
     */
    fn GetControlCenter &control = (
        local ctlCenterX = control.pos.x + (control.width / 2)
        local ctlCenterY = control.pos.y + (control.height / 2)
        
        return (Point2 ctlCenterX ctlCenterY)
    ),

    /*
     * 
     */
    fn GetRelControlPos &dialog &control = (
        local relX = (control.pos.x as float) / (dialog.width as float)
        local relY = (control.pos.y as float) / (dialog.height as float)

        return (Point2 relX relY)
    ),

    /*
     * 
     */
    fn SetControlPos &dialog &control params = (
        local dialogSize = Point2 dialog.width dialog.height
        control.pos = dialogSize * params["pos"]
    )
    
)
::DialogResizer = DialogResizerStruct()

try (DestroyDialog TestDialog) catch ()
rollout TestDialog "Test" (
    local this = TestDialog
    local ctlData = Dictionary #integer 

    button MyButton "Test Button" align:#left
    edittext text1 "" align:#left
    spinner myspinner "" align:#left

    fn OnOpen = (
        for ctl in this.controls do (
            local hwnd = (ctl.hwnd)[1] as integer
            local params = Dictionary #string #("pos", (DialogResizer.GetRelControlPos &this &ctl))

            ctlData[hwnd] = params
        )
    )

    fn OnResized size = (
        for ctl in this.controls do (
            DialogResizer.SetControlPos &this &ctl ctlData[(ctl.hwnd)[1] as integer]
        )
    )

    on TestDialog resized size do (this.OnResized size)

    on TestDialog open do (this.OnOpen())
)
CreateDialog TestDialog style:#(#style_titlebar, #style_border, #style_sysmenu, #style_resizing)
