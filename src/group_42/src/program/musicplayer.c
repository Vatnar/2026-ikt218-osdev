//
// Created by osdev on 3/23/26.
//

#include "program/frequencies.h"
#include "program/musicplayer.h"

#include <stdio.h>
#include <string.h>
#include <driver/keyboard.h>
#include <driver/keyboard_map.h>
#include <driver/PCSPK.h>
#include <kernel/pit.h>

static const note_t starwars_theme[] = {
    G4( 166 ), R(100), G4( 166 ), R(100), G4( 166 ),
    C5( 1000 ), G5( 1000 ),           // The "Big" jump
    F5( 125 ), E5( 125 ), D5( 125 ),  // Downward run
    C6( 1000 ), G5( 500 ),            // High jump
    F5( 125 ), E5( 125 ), D5( 125 ),
    C6( 1000 ), G5( 500 ),
    F5( 125 ), E5( 125 ), F5( 125 ),
    D5( 1000 )
};

static const note_t mario_theme[] = {
    // The iconic "Startup" Fanfare
    E5( 125 ), E5( 125 ), R( 125 ), E5( 125 ), R( 125 ), C5( 125 ), E5( 125 ), R( 125 ),
    G5( 250 ), R( 250 ), G4( 250 ), R( 250 ),

    // Main Theme Loop Start
    C5( 250 ), G4( 250 ), E4( 250 ),
    A4( 250 ), B4( 250 ), AS4( 125 ), A4( 250 ),

    // The "Triplet-ish" bounce
    G4( 150 ), E5( 150 ), G5( 150 ), A5( 250 ), F5( 125 ), G5( 125 ),
    R( 125 ), E5( 250 ), C5( 125 ), D5( 125 ), B4( 250 ),

    // Repeat/Next phrase
    C5( 250 ), G4( 250 ), E4( 250 ),
    A4( 250 ), B4( 250 ), AS4( 125 ), A4( 250 ),

    G4( 150 ), E5( 150 ), G5( 150 ), A5( 250 ), F5( 125 ), G5( 125 ),
    R( 125 ), E5( 250 ), C5( 125 ), D5( 125 ), B4( 250 )
};

static const note_t norwegian_anthem[] = {
    F4(1000), /* JA */ DS4(250), /* VI */ D4(500), /* EL- */ C4(500), /* SKER */
    AS3(500), /* DE- */ C4(500), /* TTE */ D4(500), /* LAN-  */ DS4(500), /* DET */
    F4(1000), /* SOM */ G4(250), /* DET */ F4(500), /* STI- */ DS4(500), /* GER */
    D4(2000), /* FREM */

    G4(1000), /* FUR- */ F4(250), /* ET */ DS4(500), /* VÆR- */ D4(500), /* BITT */
    C4(500), /* OV- */ D4(500), /* ER */ DS4(500), /* VANN */
    F4(1000), /* ET */ R(250), F4(1000), /* MED */ G4(250), /* DE */
    G4(500), /* TU */ A4(500), /* SEN */ AS4(2000), /* HJEM */
 };

static const note_t star_spangled_banner[] = {
    F4( 300 ), D4(300), AS3(600),   // o o o
    D4(600), F4(600), AS4(1200),      // can you see
    D5(600), C5(150), AS4(900),      //
    D4(600), E4(600), F4(1200),
    F4(300), F4(300), D5(1200), C5(300), AS4(600), A4(1200),
    G4(300), A4(300), AS4(600), AS4(600),
    F4(600), D4(600), AS3(600),
    F4(450), D4(150), AS3(600), D4(600), F4(600), AS4(1200),
    D5(450), C5(150), AS4(600),
    D4(600), E4(600), F4(2400), // enough i think
};

typedef struct {
    const char* name;
    const note_t* notes;
    size_t length;
} song_t;

static const song_t songs[] = {
            {        "starwars", starwars_theme, sizeof(starwars_theme) / sizeof(note_t)},
            {           "mario", mario_theme   , sizeof(mario_theme) / sizeof(note_t)},
            {           "norway", norwegian_anthem, sizeof(norwegian_anthem) / sizeof(note_t)},
            {              "usa", star_spangled_banner, sizeof(star_spangled_banner) / sizeof(note_t)},
};

#define NUM_SONGS (sizeof(songs) / sizeof(songs[0]))

static bool interrupted() {
    key_event_t ev;
    keyboard_get_event(&ev);
    const uint16_t key = keyboard_translate_no(ev);
    // control + c
    if (ev.control && key == 'c') {
        return true;
    }
    // escape
    if (ev.scancode == 0x01) {
        return true;
    }

    return false;

}

static void play_song(const song_t* song) {
    for (size_t i = 0; i < song->length; i++) {
        if (interrupted()) {
            printf("song stopped...\n");
            PCSPK_stop();
            return;
        }

        const uint32_t dur = song->notes[i].duration;

        if (song->notes[i].note > 0) {
            // Play for 90% of the duration, rest for 10%
            // This creates the 'gap' without drifting off-beat
            uint32_t play_ms = (dur * 6) / 10;
            uint32_t gap_ms  = dur - play_ms;

            PCSPK_play(song->notes[i].note);
            sleep_interrupt(play_ms);

            PCSPK_stop();
            sleep_interrupt(gap_ms);
        } else {
            // Pure Rest
            PCSPK_stop();
            sleep_interrupt(dur);
        }
    }
}


int music_player(const int argc, char** argv) {

    if (argc < 2 || strcmp(argv[1], "list") == 0) {
        // list usage / songs
        printf("Usage: music_player <song>\n");
        for (size_t i = 0; i < NUM_SONGS; i++) {
            printf("\t%s\n", songs[i].name);
        }
        return 1;
    }

    const char* song_name = argv[1];

    for (size_t i = 0; i < NUM_SONGS; i++) {
        if (strcmp(songs[i].name, song_name) == 0) {
            play_song(&songs[i]);

            return 0;
        }
    }

    printf("Unknown song: %s\n", song_name);
    printf("Type 'music_player' to see available songs.\n");
    return 1;
}