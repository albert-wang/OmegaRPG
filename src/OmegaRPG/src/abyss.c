/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* abyss.c */
/* some functions to make the abyss level and run the final challenge */

#include "glob.h"

#if defined(WIN32)
#include <sys/types.h>
#include <malloc.h>

#ifdef SAVE_LEVELS
#	include <sys/timeb.h>
#	include <io.h>
#endif
#endif

/* loads the abyss level into Level*/
void load_abyss(void)
{
    int i, j;
    char site;
    map *abyss;

    TempLevel = Level;
    if (ok_to_free(TempLevel)) {
#ifndef SAVE_LEVELS
        free_level(TempLevel);
#endif
        TempLevel = NULL;
    }
#ifndef SAVE_LEVELS
    Level = ((plv) checkmalloc(sizeof(levtype)));
#else
    msdos_changelevel(TempLevel,0,-1);
    Level = &TheLevel;
#endif

    clear_level(Level);

    abyss = map_open(MAP_abyss);
    map_setLevel(abyss,0);
    Level->level_width = map_getWidth(abyss);
    Level->level_length = map_getLength(abyss);

    for(j=0; j<Level->level_length; j++) {
        for(i=0; i<Level->level_width; i++) {
            site = map_getSiteChar(abyss,i,j);
            Level->site[i][j].roomnumber = RS_ADEPT;
            switch(site) {
            case '0':
                Level->site[i][j].locchar = VOID_CHAR;
                Level->site[i][j].p_locf = L_VOID;
                break;
            case 'V':
                Level->site[i][j].locchar = VOID_CHAR;
                Level->site[i][j].p_locf = L_VOID_STATION;
                break;
            case '1':
                Level->site[i][j].locchar = FLOOR;
                Level->site[i][j].p_locf = L_VOICE1;
                break;
            case '2':
                Level->site[i][j].locchar = FLOOR;
                Level->site[i][j].p_locf = L_VOICE2;
                break;
            case '3':
                Level->site[i][j].locchar = FLOOR;
                Level->site[i][j].p_locf = L_VOICE3;
                break;
            case '~':
                Level->site[i][j].locchar = WATER;
                Level->site[i][j].p_locf = L_WATER_STATION;
                break;
            case ';':
                Level->site[i][j].locchar = FIRE;
                Level->site[i][j].p_locf = L_FIRE_STATION;
                break;
            case '"':
                Level->site[i][j].locchar = HEDGE;
                Level->site[i][j].p_locf = L_EARTH_STATION;
                break;
            case '6':
                Level->site[i][j].locchar = WHIRLWIND;
                Level->site[i][j].p_locf = L_AIR_STATION;
                break;
            case '#':
                Level->site[i][j].locchar = WALL;
                break;
            case '.':
                Level->site[i][j].locchar = FLOOR;
                break;
            }
        }
    }
    map_close(abyss);
}


#ifdef SAVE_LEVELS
/* This stuff is in this file because the file was really small. */

void msdos_init(void)
{
    int i;

    /* Allocate the inner level of pointers for TheLevel */
    for (i = 0; i < MAXWIDTH; i++)
        TheLevel.site[i] = (plc)checkmalloc(MAXLENGTH * sizeof(loctype));

    /* Remove old level files */
    kill_all_levels();
}

void kill_all_levels(void)
{
    kill_levels("om*.lev");
}

/* Remove old level files laying around */
void kill_levels(char *str)
{
    int i;

    struct _finddata_t buf;

    sprintf(Str1, "%s%s", Omegalib, str);
    for (i = _findfirst(Str1, &buf); !i; i = _findnext(i, &buf))
    {
        sprintf(Str2, "%s%s", Omegalib, buf.name);
        remove(Str2);
    }
}

static FILE *open_levfile(int env, int depth, int rw)
{
    sprintf(Str1,"%som%03d%03d.lev",Omegalib,env,depth);
    return(fopen(Str1,(rw) ? "wb" : "rb"));
}

/* Saves oldlevel (unless NULL), and reads in the new level,
unless depth is < 0. */
plv msdos_changelevel(plv oldlevel, int newenv, int newdepth)
{
    FILE *fp;

    if (oldlevel != NULL)
    {
        if (oldlevel->environment == newenv &&
                oldlevel->depth == newdepth)
            return(oldlevel);
        if ((fp = open_levfile(oldlevel->environment,oldlevel->depth,1)) != NULL)
        {
            save_level(fp,oldlevel);
            fclose(fp);
        }
        else
            mprint("Cannot save level!!!");
        /* Free up monsters and items */
        free_level(oldlevel);
    }
    if (newdepth >= 0)
    {
        if ((fp = open_levfile(newenv,newdepth,0)) == NULL)
            return(NULL);

        restore_level(fp, VERSION);
        fclose(fp);
        return(Level);
    }
    return(NULL);
}

#endif
