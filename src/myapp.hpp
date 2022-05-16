
#ifndef MYAPP_HPP
#define MYAPP_HPP

#include "best_inversion.hpp"

using namespace al;
using namespace theory;

int nameToKeyLabelIndex(std::string name) {

  if(name == "C3")
    return 10;
  if(name == "D3")
    return 11;
  if(name == "E3")
    return 12;
  if(name == "F3")
    return 13;
  if(name == "G3")
    return 14;
  if(name == "A3")
    return 15;
  if(name == "B3")
    return 16;
  if(name == "C4")
    return 17;
  if(name == "D4")
    return 18;
  if(name == "E4")
    return 19;
  
  /*
  if(name == "F4")
    return 19;
  if(name == "G4")
    return 19;
  if(name == "A4")
    return 19;
  if(name == "B4")
    return 19;
  if(name == "C5")
    return 19;
  
  */
  if(name == "Db3")
    return 30;
  if(name == "Eb3")
    return 31;
  if(name == "Gb3")
    return 33;
  if(name == "Ab3")
    return 34;
  if(name == "Bb3")
    return 35;
  if(name == "Db4")
    return 37;
  if(name == "Eb4")
    return 38;
  /*
  if(name == "Gb4")
    return 19;
  if(name == "Ab4")
    return 19;
  if(name == "Bb4")
    return 19;

  */
  

  
  // switch (asciiK) {
  // case '2':
  //   return 30;
  // case '3':
  //   return 31;
  // case '5':
  //   return 33;
  // case '6':
  //   return 34;
  // case '7':
  //   return 35;
  // case '9':
  //   return 37;
  // case '0':
  //   return 38;

  // case 'q':
  //   return 10;
  // case 'w':
  //   return 11;
  // case 'e':
  //   return 12;
  // case 'r':
  //   return 13;
  // case 't':
  //   return 14;
  // case 'y':
  //   return 15;
  // case 'u':
  //   return 16;
  // case 'i':
  //   return 17;
  // case 'o':
  //   return 18;
  // case 'p':
  //   return 19;

  // case 's':
  //   return 20;
  // case 'd':
  //   return 21;
  // case 'g':
  //   return 23;
  // case 'h':
  //   return 24;
  // case 'j':
  //   return 25;
  // case 'l':
  //   return 27;
  // case ';':
  //   return 28;

  // case 'z':
  //   return 0;
  // case 'x':
  //   return 1;
  // case 'c':
  //   return 2;
  // case 'v':
  //   return 3;
  // case 'b':
  //   return 4;
  // case 'n':
  //   return 5;
  // case 'm':
  //   return 6;
  // case ',':
  //   return 7;
  // case '.':
  //   return 8;
  // case '/':
  //   return 9;
  // }
  return 0;
}

class MyApp : public App
{
public:

  SynthGUIManager<SquareWave> synthManager{"SquareWave"};

  Mesh meshKey;
  float keyWidth, keyHeight;
  float keyPadding = 2.f;
  float fontSize;

  Best_Inversion b_inv;

  std::string whitekeyLabels[20] = {"C3","D3","E3","F3","G3","A3","B3","C4","D4","E4",
                                    "C3","D3","E3","F3","G3","A3","B3","C4","D4","E4"};
  std::string blackkeyLabels[20] = {"Db3","Eb3","","Gb3","Ab3","Bb3","","Db4","Eb4","",
                                    "Db3","Eb3","","Gb3","Ab3","Bb3","","Db4","Eb4",""};
  // Font renderder
  FontRenderer fontRender;

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
  void onAnimate(double dt) override
  {
    // The GUI is prepared here
    imguiBeginFrame();
    // Draw a window that contains the synth control panel
    synthManager.drawSynthControlPanel();
    imguiEndFrame();
  }

  // The graphics callback function.
  // The graphics callback function.
  void onDraw(Graphics& g) override {
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
  // Whenever a key is pressed, this function is called
  bool onKeyDown(Keyboard const &k) override
  {
     if (ParameterGUI::usingKeyboard()) {  // Ignore keys if GUI is using
         return true;
    }
 
    Scale scale = Scale("C4", scale_type::Minor);
    Chord chordA = Chord("Am", 3);
    Chord chordB = Chord("Bm", 3);
    Chord chordC = Chord("Cmaj", 3);
    Chord chordD = Chord("Dm", 3);
    Chord chordE = Chord("Em", 3);
    Chord chordF = Chord("Fmaj", 3);
    Chord chordG = Chord("Gmaj", 3);
    int inversion = 0;

  if(k.shift()){
    switch (k.key())
    {
     case 'a':
      inversion = b_inv.next_best_inversion(chordA);
      chordA.invert(inversion);
      playChord(0, chordA, 1);
      for(int i=0; i<chordA.notes.size(); i++){
        std::cout<<chordA.notes[i].name()<<std::endl;
      }
      std::cout<<inversion<<std::endl;
       return false;
     case 'b':
       inversion = b_inv.next_best_inversion(chordB);
       chordB.invert(inversion); 
       playChord(0, chordB, 1);
        for(int i=0; i<chordB.notes.size(); i++){
          std::cout<<chordB.notes[i].name()<<std::endl;
        }
        std::cout<<inversion<<std::endl;
       return false;
    case 'c':
      inversion = b_inv.next_best_inversion(chordC);
      chordC.invert(inversion);
      playChord(0, chordC, 1);
      for(int i=0; i<chordC.notes.size(); i++){
          std::cout<<chordC.notes[i].name()<<std::endl;
        }
        std::cout<<inversion<<std::endl;
      return false;
    case 'd':
      inversion = b_inv.next_best_inversion(chordD);
      playChord(0, chordD, 1);
      for(int i=0; i<chordD.notes.size(); i++){
        std::cout<<chordD.notes[i].name()<<std::endl;
      }
      std::cout<<inversion<<std::endl;
      return false;
    case 'e':
      inversion = b_inv.next_best_inversion(chordE);
      playChord(0, chordE, 1);
      for(int i=0; i<chordE.notes.size(); i++){
        std::cout<<chordE.notes[i].name()<<std::endl;
      }
      std::cout<<inversion<<std::endl;

      return false;
    case 'f':
      inversion = b_inv.next_best_inversion(chordF);
      chordF.invert(inversion);
      playChord(0, chordF, 1);
      for(int i=0; i<chordF.notes.size(); i++){
        std::cout<<chordF.notes[i].name()<<std::endl;
      }
      std::cout<<inversion<<std::endl;
      return false;
    case 'g':
      inversion = b_inv.next_best_inversion(chordG);
      chordG.invert(inversion);
      playChord(0, chordG, 1);
      for(int i=0; i<chordG.notes.size(); i++){
        std::cout<<chordG.notes[i].name()<<std::endl;
      }
      std::cout<<inversion<<std::endl;

      return false; 
    }
  }
  else{

    Scale scale = Scale("C4", scale_type::Minor);
    Chord chordA = Chord("Am", 3);
    Chord chordB = Chord("Bm", 3);
    Chord chordC = Chord("Cmaj", 3);
    Chord chordD = Chord("D", 3);
    Chord chordE = Chord("Em", 3);
    Chord chordF = Chord("Fmaj", 3);
    Chord chordG = Chord("Gmaj", 3);
    Note A4 = Note('C', '#', 4);

    switch (k.key())
    {
     case 'a':
      playChord(0, chordA, 1);
      for(int i=0; i<chordA.notes.size(); i++){
        std::cout<<chordA.notes[i].name()<<std::endl;
        }
       return false;
     case 'b':
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
      playChord(0, chordF, 1);
      for(int i=0; i<chordF.notes.size(); i++){
        std::cout<<chordF.notes[i].name()<<std::endl;
      }
      return false;
    case 'g':
      playChord(0, chordG, 1);
      for(int i=0; i<chordG.notes.size(); i++){
        std::cout<<chordG.notes[i].name()<<std::endl;
      }
      return false; 
    }
    
  }
    return true;
  }

  // Whenever a key is released this function is called
  bool onKeyUp(Keyboard const &k) override{
   int midiNote = asciiToMIDI(k.key());
    if (midiNote > 0)
    {
      synthManager.triggerOff(midiNote);
    }
    return true;
  }

   // Whenever the window size changes this function is called
  void onResize(int w, int h) override {
    // Recaculate the size of piano keys based new window size
    keyWidth = w / 10.f - keyPadding * 2.f;
    keyHeight = h / 2.f - keyPadding * 2.f;
    fontSize = keyWidth * 0.2;
    addRect(meshKey, 0, 0, keyWidth, keyHeight);
  }

  void onExit() override { imguiShutdown(); }

  // New code: a function to play a note A

  float playNote(float time, Note note, float duration = 0.5, float amp = 0.2, float attack = 0.1, float decay = 0.5)
  {
      int midiNote = note.midi();
      
      //if (midiNote > 0) {
        // Check which key is pressed
        int keyIndex = nameToKeyLabelIndex(note.name());
        
        bool isBlackKey = false;
        if(keyIndex >= 20) {
          keyIndex -= 20;
          isBlackKey = true;
        }

        //synthManager.voice()->setInternalParameterValue(
        //    "frequency", ::pow(2.f, (midiNote - 69.f) / 12.f) * 432.f);

        float w = keyWidth;
        float h = keyHeight;
        float x = (keyWidth + keyPadding * 2) * (keyIndex % 10) + keyPadding;
        float y = 0;
        
        if(isBlackKey == true) {
          x += keyWidth * 0.5;
          y += keyHeight * 0.5;
          h *= 0.5;
        }

        std::cout<<"w = "<<w<<std::endl;
        std::cout<<"h = "<<h<<std::endl;
        std::cout<<"x = "<<x<<std::endl;
        std::cout<<"y = "<<y<<std::endl;
        
        if(keyIndex >= 10) {
          y += keyHeight + keyPadding * 2;
        }
      // synthManager.voice()->setInternalParameterValue("pianoKeyWidth", w);
      // synthManager.voice()->setInternalParameterValue("pianoKeyHeight", h);
      // synthManager.voice()->setInternalParameterValue("pianoKeyX", x);
      // synthManager.voice()->setInternalParameterValue("pianoKeyY", y);

    auto *voice = synthManager.synth().getVoice<SquareWave>();
    // amp, freq, attack, release, pan
    voice->setTriggerParams({amp, note.frequency(), 0.1, 0.1, 0.0, x, y,w,h});
    synthManager.synthSequencer().addVoiceFromNow(voice, time, duration*0.9);
     // }
    return time+duration;
  
  }

  float playChord(float time, Chord chord, float duration, bool roll=false){
      float localTime = 0;
      for(int i=0; i<chord.notes.size(); i++){
          playNote(time+localTime, chord.notes[i], duration, 0.05, 0.2, 0.75);
          if(roll) localTime += duration / 32;
      }
      return time+duration;
  }

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