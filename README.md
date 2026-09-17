# Umbra Nota
A fast stylized quicknote tool

## Goal
The goal is to make a quick note program where once can create a note in a very short amount of time.

### Todo
#### Visuals
- [ ] Themes
- [ ] Background Image
- [ ] Round Rectangles
- [ ] Clip overflow note text in Notepad
- [ ] Mask overflow note in scroll view
- [ ] Hover effect only when program in focus
- [ ] Transparent Text Prompt
	  
#### Functionality
- [ ] Settings
	- [ ] Open On Start
	- [ ] Keybinds
- [ ] Auto Tag
	- [ ] Ruleset management ( OnContext: "Program Name", SetCategory: X, AddTags: X, AttachFile: Screenshot, Etc...)
- [ ] Definitions ( Words the Send you to another Note )

#### Note Editing
- [ ] Add Tag
- [ ] Edit Head
- [ ] Edit Body
- [ ] Trash Note
- [ ] Delete

#### Text Editing
- [ ] CTRL + V Pasting
- [x] CTRL + Backspace
- [ ] CTRL + Del
- [x] Caret, Insertion Line
- [x] Blinking Caret
- [ ] Click to Insert Caret
- [ ] Arrow Keys Move Caret
	- Missing: Vertical Adjustment
- [x] Delete Key
- [ ] Text Selection
	- [ ] CTRL + A
	- [ ] SHIFT + ARROW
	- [ ] Mouse Drag
- [ ] SHFT + Enter Next Line
	- Missing: Visual Support
	- Missing: Caret Support

#### Note Metadata
- [x] Created At
- [x] Last Modified
- [x] Tags
- [ ] Category
- [ ] Context
- [ ] Attached File
- [ ] Location
- [ ] Viewed Since Creation

#### File Management
- [x] Save Note
- [x] Save Note Metadata
- [x] Save after Note Added
- [ ] Save notes individually ( n\{id\}.unn ) Note \{id\} . Umbra Nota Note
- [ ] Save ID List for .unn files ( nl.unl ) Note List . Umbra Nota List
- [ ] Encoding / Decoding to use less storage

#### Notepad
- [x] Display Notes
- [x] Scroll View
- [x] Click to View
- [ ] Add Note from Notepad
- [x] Show: Metadata
- [x] Show \#tags
- [ ] Grid Layout
- [ ] Sorting
	- [ ] Multi Filter Sorting
	- [ ] None
	- [ ] Move Up
	- [ ] Move Down
	- [ ] By Viewed 
	- [ ] By Category
	- [ ] By Date Created
	- [ ] By Last Modified ( Default )
	- [ ] By Tag
	- [ ] By Category
	- [ ] By Header
	- [ ] By Content
	- [ ] By Context

#### Internal
- [ ] Personal UI Library
    - [ ] Static Styles
    - [x] Rect
	- [ ] Text Handler
		- [ ] Line Wrap
				- Missing: Proper support
		- [ ] Text Formatting ( Mark Down )
		- [ ] Text Decoration ( \<attr=value\> Content \<\/attr\> )
		- [ ] Text Selection
		- [ ] CTRL + C Copy
	- [ ] Container
			- Missing: Draw from class
	- [ ] Scroll View
			- Missing: Draw from class
			- Missing: Scroll Bar ( idea: use slider )
			- Missing: Contest Defined Scroll Boundaries
	- [x] Input Field
	- [ ] Drop Down
	- [ ] Check Box
	- [ ] Slider