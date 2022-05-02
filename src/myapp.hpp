#ifndef MYAPP_HPP
#define MYAPP_HPP

#include "Gamma/Analysis.h"
#include "Gamma/Effects.h"
#include "Gamma/Envelope.h"
#include "Gamma/Oscillator.h"

#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/scene/al_PolySynth.hpp"
#include "al/scene/al_SynthSequencer.hpp"
#include "al/ui/al_ControlGUI.hpp"
#include "al/ui/al_Parameter.hpp"

#include "al/graphics/al_Shapes.hpp"
#include "al/graphics/al_Font.hpp"

#include "squarewave.hpp"

#include "theory.hpp"
#include "tempo.hpp"



using namespace al;
using namespace theory;

// The helper function used to visualize which keys pressed or released on a virtual piano.
int asciiToKeyLabelIndex(int asciiKey) {
  switch (asciiKey) {
  case '2':
    return 30;
  case '3':
    return 31;
  case '5':
    return 33;
  case '6':
    return 34;
  case '7':
    return 35;
  case '9':
    return 37;
  case '0':
    return 38;

  case 'q':
    return 10;
  case 'w':
    return 11;
  case 'e':
    return 12;
  case 'r':
    return 13;
  case 't':
    return 14;
  case 'y':
    return 15;
  case 'u':
    return 16;
  case 'i':
    return 17;
  case 'o':
    return 18;
  case 'p':
    return 19;

  case 's':
    return 20;
  case 'd':
    return 21;
  case 'g':
    return 23;
  case 'h':
    return 24;
  case 'j':
    return 25;
  case 'l':
    return 27;
  case ';':
    return 28;

  case 'z':
    return 0;
  case 'x':
    return 1;
  case 'c':
    return 2;
  case 'v':
    return 3;
  case 'b':
    return 4;
  case 'n':
    return 5;
  case 'm':
    return 6;
  case ',':
    return 7;
  case '.':
    return 8;
  case '/':
    return 9;
  }
  return 0;
}

// We make an app.
class MyApp : public App {
 public:
  // GUI manager for SineEnv voices
  // The name provided determines the name of the directory
  // where the presets and sequences are stored
  SynthGUIManager<SquareWave> synthManager{"Chord Graphics"};
  
  // Mesh and variables for drawing piano keys
  Mesh meshKey;
  float keyWidth, keyHeight;
  float keyPadding = 2.f;
  float fontSize;
  std::string whitekeyLabels[20] = {"C3","D3","E3","F3","G3","A3","B3","C4","D4","E4",
                                    "C5","D5","E5","F5","G5","A5","B5","C6","D6","E6"};
  std::string blackkeyLabels[20] = {"Db3","Eb3","","Gb3","Ab3","Bb3","","Db4","Eb4","",
                                    "Db5","Db5","","Gb5","Ab5","Bb5","","Db6","Eb6",""};
  // Font renderder
  FontRenderer fontRender;

  // This function is called right after the window is created
  // It provides a grphics context to initialize ParameterGUI
  // It's also a good place to put things that should
  // happen once at startup.
  void onCreate() override {
    navControl().active(false);  // Disable navigation via keyboard, since we
                                 // will be using keyboard for note triggering

    // Set sampling rate for Gamma objects from app's audio
    gam::sampleRate(audioIO().framesPerSecond());

    imguiInit();

    // Cacluate the size of piano keys based on the app window size
    float w = float(width());
    float h = float(height());
    keyWidth = w / 10.f - keyPadding * 2.f;
    keyHeight = h / 2.f - keyPadding * 2.f;
    fontSize = keyWidth * 0.2;

    // Create a mesh that will be drawn as piano keys
    addRect(meshKey, 0, 0, keyWidth, keyHeight);

    // Set the font renderer
    fontRender.load(Font::defaultFont().c_str(), 60, 1024);

    synthManager.synthRecorder().verbose(true);
  }
  // The audio callback function. Called when audio hardware requires data
  void onSound(AudioIOData& io) override {
    synthManager.render(io);  // Render audio
  }

  void onAnimate(double dt) override{
    // The GUI is prepared here
    imguiBeginFrame();
    // Draw a window that contains the synth control panel
    synthManager.drawSynthControlPanel();
    imguiEndFrame();
  }

  // The graphics callback function.
   void onDraw(Graphics& g) override{
    g.clear();

    // This example uses only the orthogonal projection for 2D drawing
    g.camera(Viewpoint::ORTHO_FOR_2D);  // Ortho [0:width] x [0:height]

    // Drawing white piano keys
    for(int i = 0; i < 20; i++) {
      int index = i % 10;
      g.pushMatrix();
      
      float c = 0.9;
      float x = (keyWidth + keyPadding * 2) * index + keyPadding;
      float y = 0;

      if(i >= 10) {
        y = keyHeight + keyPadding * 2;
      }

      g.translate(x, y);
      g.color(c);
      g.tint(c);
      g.draw(meshKey);
      
      g.tint(1);
      fontRender.write(whitekeyLabels[i].c_str(), fontSize);
      fontRender.renderAt(g, {keyWidth * 0.5 - 5, keyHeight * 0.1, 0.0});

      g.popMatrix();
    }

    // Drawing balck piano keys
    for(int i = 0; i < 20; i++) {
      int index = i % 10;
      if(index == 2 || index == 6 || index == 9) continue;

      g.pushMatrix();
      
      float c = 0.5;      
      float x = (keyWidth + keyPadding * 2) * index + keyPadding + keyWidth * 0.5;
      float y = keyHeight * 0.5;

      if(i >= 10) {
        y = y + keyHeight + keyPadding * 2;
      }

      g.translate(x, y);
      g.scale(1, 0.5);
      g.color(c);
      g.tint(c);
      g.draw(meshKey);
      
      g.scale(1, 2);
      
      g.tint(1);
      fontRender.write(blackkeyLabels[i].c_str(), fontSize);
      fontRender.renderAt(g, {keyWidth * 0.5 - 5, keyHeight * 0.1, 0.0});
      
      g.popMatrix();
    }

    // Render the synth's graphics
    synthManager.render(g);

    // GUI is drawn here
    imguiDraw();
  }

//   // Whenever a key is pressed, this function is called
   bool onKeyDown(Keyboard const& k) override{
    
    if (ParameterGUI::usingKeyboard()) {  // Ignore keys if GUI is using
         return true;
    }
 
    Scale scale = Scale("C4", scale_type::Minor);
    Chord chordA = Chord("Am", 2);
    Chord chordB = Chord("Bm", 2);
    Chord chordC = Chord("Cmaj", 3);
    Chord chordD = Chord("Dm", 3);
    Chord chordE = Chord("Em", 3);
    Chord chordF = Chord("Fmaj", 2);
    Chord chordG = Chord("Gmaj", 2);

    switch (k.key())
    {
     case 'a':
      chordA.invert(1);
      playChord(0, chordA, 1);
      for(int i=0; i<chordA.notes.size(); i++){
        std::cout<<chordA.notes[i].name()<<std::endl;
      }
       return false;
     case 'b':
       chordB.invert(2); 
       playChord(0, chordB, 1);
        for(int i=0; i<chordB.notes.size(); i++){
          std::cout<<chordB.notes[i].name()<<std::endl;
        }
       return false;
    case 'c':
      playChord(0, chordC, 1);
      for(int i=0; i<chordC.notes.size(); i++){
          std::cout<<chordC.notes[i].name()<<std::endl;
        }
      return false;
    case 'd':
      playChord(0, chordD, 1);
      for(int i=0; i<chordD.notes.size(); i++){
        std::cout<<chordD.notes[i].name()<<std::endl;
      }
      return false;
    case 'e':
      playChord(0, chordE, 1);
      for(int i=0; i<chordE.notes.size(); i++){
        std::cout<<chordE.notes[i].name()<<std::endl;
      }
      return false;
    case 'f':
      chordF.invert(2);
      playChord(0, chordF, 1);
      for(int i=0; i<chordF.notes.size(); i++){
        std::cout<<chordF.notes[i].name()<<std::endl;
      }
      return false;
    case 'g':
      chordG.invert(2);
      playChord(0, chordG, 1);
      for(int i=0; i<chordG.notes.size(); i++){
        std::cout<<chordG.notes[i].name()<<std::endl;
      }
      return false; 
    }
    return true;
  }

  // Whenever a key is released this function is called
  bool onKeyUp(Keyboard const& k) override{
    
     int midiNote = asciiToMIDI(k.key());
    if (midiNote > 0)
    {
      synthManager.triggerOff(midiNote);
    }
    return true;
  }
  
  // Whenever the window size changes this function is called
  void onResize(int w, int h) override{
    // Recaculate the size of piano keys based new window size
    keyWidth = w / 10.f - keyPadding * 2.f;
    keyHeight = h / 2.f - keyPadding * 2.f;
    fontSize = keyWidth * 0.2;
    addRect(meshKey, 0, 0, keyWidth, keyHeight);
  }

  void onExit() override;// { imguiShutdown(); }

  float playNote(float time, Note note, float duration = 0.5, float amp = 0.2, float attack = 0.1, float decay = 0.5)
  {
    auto *voice = synthManager.synth().getVoice<SquareWave>();
    // amp, freq, attack, release, pan
    voice->setTriggerParams({amp, note.frequency(), 0.1, 0.1, 0.0});
    synthManager.synthSequencer().addVoiceFromNow(voice, time, duration*0.9);

    return time+duration;

  }

//   void releaseNote(Note note)
//   {
//       int midiNote = note.midi();
//       if (midiNote > 0) {
//         synthManager.triggerOff(midiNote);
//       }
//   }
  float playChord(float time, Chord chord, float duration, bool roll=false)
 {
      float localTime = 0;
      for(int i=0; i<chord.notes.size(); i++){
          playNote(time+localTime, chord.notes[i], duration, 0.05, 0.2, 0.75);
          if(roll) localTime += duration / 32;
      }
      return time+duration;
  }
  // void releaseChord(Chord chord){
  //   float localTime = 0;
  //     for(int i=0; i<chord.notes.size(); i++){
  //         releaseNote(chord.notes[i]);
  //     }
  // }
  void playHappyBirthday(Note root, float bpm)
  {
    // Happy birthday uses: P1(C), M2(D), M3(E), P4(F), P5(G), M6(A), and m7(Bb)
    Scale majScale = Scale(root, scale_type::Major);
    Note M2 = majScale.degree(scale_type::II);
    Note M3 = majScale.degree(scale_type::III);
    Note P4 = majScale.degree(scale_type::IV);
    Note P5 = majScale.degree(scale_type::V);
    Note M6 = majScale.degree(scale_type::VI);
    Note m7 = root.interval(interval_type::m7);
    Note P8 = majScale.degree(scale_type::VIII);

    // For chords it needs: F Maj, C Dom7, Bb Maj, F/C (2nd inversion)
    // (Also dropping the root of the chord down an octave or Perfect Eighth)

    Chord chord1 = P4.octaveDown().chord("maj");

    Chord chord2 = root.chord("7");

    Chord chord3 = m7.octaveDown().chord("maj");

    Chord chord4 = P4.octaveDown().chord("maj"); // 2nd inversion
    chord4.invert(2);

    chord2.match(chord1);
    chord3.match(chord1);
    chord4.match(chord1);


    // Now lets set up a tempo
    //  syntax: Tempo(bpm, timeSig top, timeSig bottom)
    float time = 0;
    Tempo tpo(bpm, 3, 4); 
    // this allows us to say get exact durations for common note types

    time = playNote(time, root, tpo.duration(Tempo::eighth, true)); // true = dotted note
    time = playNote(time, root, tpo.duration(Tempo::sixteenth)); 

    playChord(time, chord1, tpo.duration(Tempo::half));
    time = playNote(time, M2, tpo.duration(Tempo::quarter)); 
    time = playNote(time, root, tpo.duration(Tempo::quarter)); 
    time = playNote(time, P4, tpo.duration(Tempo::quarter)); 

    playChord(time, chord2, tpo.duration(Tempo::half));
    time = playNote(time, M3, tpo.duration(Tempo::half)); 
    time = playNote(time, root, tpo.duration(Tempo::eighth, true)); // true = dotted note
    time = playNote(time, root, tpo.duration(Tempo::sixteenth));

    time = playNote(time, M2, tpo.duration(Tempo::q)); 
    time = playNote(time, root, tpo.duration(Tempo::q)); 
    time = playNote(time, P5, tpo.duration(Tempo::q)); 

    playChord(time, chord1, tpo.duration(Tempo::h));
    time = playNote(time, P4, tpo.duration(Tempo::h)); 
    time = playNote(time, root, tpo.duration(Tempo::e, true)); 
    time = playNote(time, root, tpo.duration(Tempo::s));

    time = playNote(time, P8, tpo.duration(Tempo::q)); 
    time = playNote(time, M6, tpo.duration(Tempo::q)); 
    time = playNote(time, P4, tpo.duration(Tempo::q)); 

    playChord(time, chord3, tpo.duration(Tempo::h));
    time = playNote(time, M3, tpo.duration(Tempo::q)); 
    time = playNote(time, M2, tpo.duration(Tempo::q)); 
    time = playNote(time, m7, tpo.duration(Tempo::e, true));
    time = playNote(time, m7, tpo.duration(Tempo::s));

    playChord(time, chord4, tpo.duration(Tempo::h));
    time = playNote(time, M6, tpo.duration(Tempo::q)); 
    time = playNote(time, P4, tpo.duration(Tempo::q)); 
    playChord(time, chord2, tpo.duration(Tempo::q));   
    time = playNote(time, P5, tpo.duration(Tempo::q));  

    playChord(time, chord1, tpo.duration(Tempo::h));
    time = playNote(time, P4, tpo.duration(Tempo::h)); 

  }

};


#endif