struct DialogResizerStruct (
private

    controlData = Dictionary #integer,

public

	/*
     * Returns the top left position of the control within the dialog. This is an absolute
     * XY coordinate of the control's top-left point.
     * @param {RolloutControl} &control - The control to get the position of.
     * @returns {Point2}
     */
	fn GetControlTopLeft &control = (
		return Point2 control.pos[1] control.pos[2]
	),

	/*
     * Returns the top right position of the control within the dialog. This is an absolute
     * XY coordinate of the control's top-right point.
     * @param {RolloutControl} &control - The control to get the position of.
     * @returns {Point2}
     */
	fn GetControlTopRight &control = (
		local ctlTopRightX = control.pos.x + control.width
		local ctlTopRightY = control.pos.y + control.height
		
		return (Point2 ctlCenterX ctlCenterY)
	),

	/*
     * Returns the center position of the control within the dialog. This is an absolute
     * XY coordinate of the control's center point.
     * @param {RolloutControl} &control - The control to get the center position of.
     * @returns {Point2}
     */
	fn GetControlCenter &control = (
		local ctlCenterX = control.pos.x + (control.width / 2)
		local ctlCenterY = control.pos.y + (control.height / 2)
		
		return (Point2 ctlCenterX ctlCenterY)
	),

	/*
     * Returns the ???
     * @param {RolloutControl} &control - The control to get the position of.
     * @returns {Point2}
     */
	fn GetRelControlPos &dialog &control = (
		local relX = (control.pos.x as float) / (dialog.width as float)
		local relY = (control.pos.y as float) / (dialog.height as float)

		return (Point2 relX relY)
	),

	/*
     * Returns the determined alignment of the control, relative to its parent dialog,
     * as the alignment is not saved in the control's parameters.
     * @param {Rollout} &dialog - The parent dialog of the control.
     * @param {RolloutControl} &control - The control to get the alignment of.
     * @returns {Name}
     */
	fn GetControlAlignment &dialog &control = (
		local alignment
		local ctlRelPos = GetRelControlPos &dialog &control

		if (ctlRelPos.x < 0.33) then (alignment = #left)
		if (ctlRelPos.x >= 0.33 and CtlRelPos.x >= 0.66) then (alignment = #center)
		if (ctlRelPos.x > 0.66) then (alignment = #right)

		return alignment
	),

	/*
     * 
     */
	fn SetControlPos &dialog &control = (
		local dialogSize = Point2 dialog.width dialog.height
		control.pos = dialogSize * controlData[(control.hwnd)[1] as integer][#pos]
    ),

    /*
     * 
     */
    fn GetControlData &control = (
        handle = control.hwnd[1] as integer
        return (controlData[handle])
    ),

    fn StoreControlData &dialog &controls = (
        -- For each control...
		for ctl in controls do (
            -- Get the control hwnd handle; this will be used to reference this control
            -- when trying to access this control
            local handle = ctl.hwnd[1] as integer
            
            -- Get the control's relevant properties we want to store
			local params = Dictionary #name #(#pos, (DialogResizer.GetRelControlPos &dialog &ctl))

            -- Store the control properties
			controlData[handle] = params
		)
    )
)
::DialogResizer = DialogResizerStruct()

try (DestroyDialog TestDialog) catch ()
rollout TestDialog "Test" (
	local this = TestDialog

    group "Test!" (
        edittext text1 "" align:#left width:200 across:2
        button MyButton "..." align:#right
    )

    group "Group 2" (
        listBox myListBox "List box!" width:200
        button myButton2 "Add" across:2 width:60
        button myButton3 "Remove" width:60
    )


	fn OnOpen = (DialogResizer.StoreControlData &this &this.controls)

	fn OnResized size = (
		for ctl in this.controls do (
			DialogResizer.SetControlPos &this &ctl
		)
    )
    
    fn OnMyButtonPressed = (
        for ctl in this.controls do (
            data = DialogResizer.GetControlData &ctl
            print (data)

            alignment = DialogResizer.GetControlAlignment &this &ctl
            print (alignment)
        )
    )

	-- on TestDialog resized size do (this.OnResized size)
    on MyButton pressed do (OnMyButtonPressed())

	on TestDialog open do (this.OnOpen())
)
CreateDialog TestDialog style:#(#style_titlebar, #style_border, #style_sysmenu, #style_resizing)
