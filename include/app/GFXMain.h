//
//  GFXMain.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/app/KeyEvent.h"

#ifdef __linux
    //linux
    #include "../data/Model.h"
    #include "../data/GFXGlobal.h"
    #include "../data/SerialReader.h"
    #include "../data/StateManager.h"

    #include "../views/NavBarView.h"
    #include "../views/RaceView.h"
    #include "../views/RosterView.h"
    #include "../views/SettingsView.h"
#else
    // Windows & OSX
    #include "data/Model.h"
    #include "data/GFXGlobal.h"
    #include "data/SerialReader.h"
    #include "data/StateManager.h"

    #include "views/NavBarView.h"
    #include "views/RaceView.h"
    #include "views/RosterView.h"
    #include "views/SettingsView.h"
#endif

namespace gfx{

    using GFXMainRef = std::shared_ptr<class GFXMain>;

    class GFXMain
    {
      public:
        GFXMain();
        ~GFXMain();
        void setup();
        void update();
        void draw( const ci::Rectf &drawRect );

        void resetPlayerData();

      private:
        void onAppStateChaged( APP_STATE as );
        void onRaceStateChanged( RACE_STATE rc );

        void onRaceFinished();

        GFXGlobal       *mGlobal;

        NavBarViewRef       mNav;
        RaceViewRef         mRaceView;
        RosterViewRef       mRosterView;
        SettingsViewRef     mSettingsView;

        Model               *mModel;
        SerialReaderRef     mSerialReader;
        StateManager        *mStateManager;

        void onKeyDown( ci::app::KeyEvent event );
    };

}
