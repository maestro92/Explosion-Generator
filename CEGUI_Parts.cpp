

/*
void main_loop()
{
  bool must_quit = false;

  // get "run-time" in seconds
  double last_time_pulse = 0.001*static_cast<double>(SDL_GetTicks());

  while (!must_quit)
  {
    inject_input(must_quit);
    inject_time_pulse(last_time_pulse);
    render_gui();
  }
}


void inject_input(bool& must_quit)
{
  SDL_Event e;

  // go through all available events
  while (SDL_PollEvent(&e))
  {
    // we use a switch to determine the event type
    switch (e.type)
    {
      // mouse motion handler
      case SDL_MOUSEMOTION:
        // we inject the mouse position directly.
        CEGUI::System::getSingleton().injectMousePosition(
          static_cast<float>(e.motion.x),
          static_cast<float>(e.motion.y)
        );
        break;

      // mouse down handler
      case SDL_MOUSEBUTTONDOWN:
        // let a special function handle the mouse button down event
        handle_mouse_down(e.button.button);
        break;

      // mouse up handler
      case SDL_MOUSEBUTTONUP:
        // let a special function handle the mouse button up event
        handle_mouse_up(e.button.button);
        break;


      // key down
      case SDL_KEYDOWN:
        // to tell CEGUI that a key was pressed, we inject the scancode.
        CEGUI::System::getSingleton().injectKeyDown(e.key.keysym.scancode);

        // as for the character it's a litte more complicated. we'll use for translated unicode value.
        // this is described in more detail below.
        if ((e.key.keysym.unicode != 0)
        {
          CEGUI::System::getSingleton().injectChar(e.key.keysym.unicode);
        }
        break;

      // key up
      case SDL_KEYUP:
        // like before we inject the scancode directly.
        CEGUI::System::getSingleton().injectKeyUp(e.key.keysym.scancode);
        break;


      // WM quit event occured
      case SDL_QUIT:
        must_quit = true;
        break;

    }

  }

}
*/


void handle_mouse_down(Uint8 button)
{
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();

	switch ( button ) {
		case SDL_BUTTON_LEFT:
			context.injectMouseButtonDown(CEGUI::LeftButton);
			break;
		case SDL_BUTTON_MIDDLE:
			context.injectMouseButtonDown(CEGUI::MiddleButton);
			break;
		case SDL_BUTTON_RIGHT:
			context.injectMouseButtonDown(CEGUI::RightButton);
			break;

		case SDL_BUTTON_WHEELDOWN:
			context.injectMouseWheelChange( -1 );
			break;
		case SDL_BUTTON_WHEELUP:
			context.injectMouseWheelChange( +1 );
			break;
	}

}

void handle_mouse_up(Uint8 button)
{
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();

    switch ( button )
        {
        case SDL_BUTTON_LEFT:
            context.injectMouseButtonUp(CEGUI::LeftButton);
            break;
        case SDL_BUTTON_MIDDLE:
            context.injectMouseButtonUp(CEGUI::MiddleButton);
            break;
        case SDL_BUTTON_RIGHT:
            context.injectMouseButtonUp(CEGUI::RightButton);
            break;
        }
}



void inject_input (bool & must_quit)
{
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();

	SDL_Event e;
	/* go through all available events */
	while (SDL_PollEvent(&e)) {
		/* we use a switch to determine the event type */
		switch (e.type) {
			/* mouse motion handler */
			case SDL_MOUSEMOTION:
				/* we inject the mouse position directly. */
				context.injectMousePosition(static_cast<float>(e.motion.x),static_cast<float>(e.motion.y));
				break;

			/* mouse down handler */
			case SDL_MOUSEBUTTONDOWN:
				/* let a special function handle the mouse button down event */
				handle_mouse_down (e.button.button);
				break;

			/* mouse up handler */
			case SDL_MOUSEBUTTONUP:
				/* let a special function handle the mouse button up event */
				handle_mouse_up (e.button.button);
				break;

			/* key down */
			case SDL_KEYDOWN:
				/* to tell CEGUI that a key was pressed, we inject the scancode. */

				CEGUI::InjectedInputReceiver::injectKeyDown( e.key.keysym.scancode );

				/* as for the character it's a litte more complicated.
				 * we'll use for translated unicode value.
				 * this is described in more detail below.
				 */
				if ((e.key.keysym.unicode & 0xFF80) == 0) {
					CEGUI::System::getSingleton().injectChar(e.key.keysym.unicode & 0x7F);
				}
				break;

			// key up
			case SDL_KEYUP:
				/* like before we inject the scancode directly. */
				CEGUI::System::getSingleton().injectKeyUp(e.key.keysym.scancode);
				break;

			// WM quit event occured
			case SDL_QUIT:
				must_quit = true;
				break;

			case SDL_VIDEORESIZE:
				CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Size(e.resize.w,e.resize.h));
				break;
		}
	}
}





void inject_time_pulse(double& last_time_pulse)
{
	// get current "run-time" in seconds
	double t = 0.001*SDL_GetTicks();

	// inject the time that passed since the last call
	CEGUI::System::getSingleton().injectTimePulse( float(t-last_time_pulse) );

	// store the new time as the last time
	last_time_pulse = t;
}

void render_gui()
{
	// clear the colour buffer
	glClear( GL_COLOR_BUFFER_BIT );

	// render the GUI :)
	CEGUI::System::getSingleton().renderAllGUIContexts();

	// Update the screen
	SDL_GL_SwapBuffers();
}
