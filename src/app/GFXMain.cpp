//
//  GFXMain.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#ifdef __linux
    //linux
    #include "../../include/app/GFXMain.h"
#else
    // Windows & OSX
    #include "app/GFXMain.h"
#endif


using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

GFXMain::GFXMain(){

}

GFXMain::~GFXMain(){
    CI_LOG_I("GFX main shutting down");
}

void GFXMain::setup(){
    // INIT --------------------------------------------------------------
    mModel = Model::getInstance();
    mGlobal = GFXGlobal::getInstance();

    mSerialReader = SerialReaderRef( new SerialReader() );
    mSerialReader->setup();

    mStateManager = StateManager::getInstance();

    // VIEWS --------------------------------------------------------------
    mNav = NavBarViewRef(new NavBarView());
    mRaceView = RaceViewRef(new RaceView());
    mRosterView = RosterViewRef(new RosterView());
    mSettingsView = SettingsViewRef(new SettingsView());

    mNav->setup();
    mRaceView->setup();
    mRosterView->setup();

    // EVENTS --------------------------------------------------------------
    getWindow()->getSignalKeyDown().connect(std::bind(&GFXMain::onKeyDown, this, std::placeholders::_1));

    mStateManager->signalOnRaceFinished.connect( bind( &GFXMain::onRaceFinished, this ) );

    mStateManager->signalOnStateChange.connect( bind( &GFXMain::onAppStateChaged, this, std::placeholders::_1 ) );
    mStateManager->signalOnRaceStateChange.connect( bind( &GFXMain::onRaceStateChanged, this, std::placeholders::_1 ) );

    mStateManager->signalRacerFinish.connect( [&](int _id, int _millis){
        mModel->playerData[_id]->setFinished(_millis);
    });

    // START --------------------------------------------------------------
    mStateManager->changeAppState( APP_STATE::RACE, true );
    mStateManager->changeRaceState( RACE_STATE::RACE_STOPPED, true );
}

void GFXMain::onRaceFinished() {
    console() << "GFXMAIN :: RACE FINSIHED" << endl;
    mSerialReader->stopRace();
    mStateManager->changeRaceState( RACE_STATE::RACE_COMPLETE );
}

void GFXMain::onAppStateChaged( APP_STATE as ) {

}

void GFXMain::onRaceStateChanged( RACE_STATE rc ){

    if( rc == RACE_STATE::RACE_STARTING ){
		mModel->resetPlayers();
        mSerialReader->setRaceLengthTicks( mModel->getRaceLengthTicks() );
        mSerialReader->startRace();
    }

    else if( rc == RACE_STATE::RACE_STOPPED ){
        mSerialReader->stopRace();
     //   mModel->resetPlayers();
        mModel->elapsedRaceTimeMillis = 0.0;
    }
}

void GFXMain::onKeyDown(KeyEvent event){
    if( !event.isAccelDown() && !event.isControlDown() ){
        return;
    }
    else if( event.getChar() == 'r'){
        mRaceView->reloadShader();
    }
    else if( event.getChar() == 'm'){
        mSerialReader->setMockMode();
    }
}

void GFXMain::resetPlayerData(){
    for( int i=0; i<mModel->playerData.size(); i++){
        mModel->playerData[i]->reset();
        mModel->playerData[i]->setRollerDiameter(mModel->getRollerDiameterMm());
    }
}

void GFXMain::update()
{
    double start_t, end_t;
    start_t = getElapsedSeconds();
    mSerialReader->update();
    end_t = getElapsedSeconds();
//    console() << "SerialReader update time :: " << start_t << " -- " << (end_t - start_t) << endl;

    mRaceView->update();
    mRosterView->update();
    mSettingsView->update();
}

void GFXMain::draw( const Rectf &drawRect ){
//    mGlobal->setScale(drawRect);

    mRaceView->draw();
    mRosterView->draw();
    mSettingsView->draw();

    mNav->draw();
}
