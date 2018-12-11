# - *- coding: utf-8 - *-

import epd2in7b
from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw
import datetime

import textwrap

import pandas as pd
pd.set_option('display.max_columns', 10)

import requests
import json


COLORED = 1
UNCOLORED = 0

epd = epd2in7b.EPD()
epd.init()

def main():
    
#--------------------------------------------------    
    # Get today's date
    now = datetime.datetime.now()
    today = now.strftime("%d %B, %A")
    #today = u'Trečiadienis'
    
    datadump = requests.get(u'http://eismoinfo.lt/weather-conditions-service')
    data = json.loads(datadump.text)
    df = pd.DataFrame(data)

    details_list = ['surinkimo_data', 'irenginys', 'krituliu_tipas', 'krituliu_kiekis', 'dangos_temperatura', 'kelio_danga', 'sukibimo_koeficientas', 'matomumas', 'vejo_greitis_maks']
    # Get first 7 device's data on A1 from Vilnius to XI Fortas
    all_data = df.loc[df['numeris']=='A1', details_list].head(7)
    # Get first 2 device's data on A5 from Vilijampole to Garliava
    a5_data = df.loc[df['numeris']=='A5',details_list].head(2)
    all_data = all_data.append(a5_data, ignore_index=True)
    all_data = all_data.fillna(u'Nėra')
    
    
    vilnius = all_data.loc[0,'irenginys'] + "    " + all_data.loc[0,'surinkimo_data'] + "\n" + all_data.loc[0,'krituliu_tipas'] + "  " + all_data.loc[0,'sukibimo_koeficientas'] + "  " + all_data.loc[0,'kelio_danga'] + "  " + all_data.loc[0,'dangos_temperatura'] + " C"
    didziulis = all_data.loc[1,u'irenginys'] + "    " + all_data.loc[1,'surinkimo_data'] + "\n" + all_data.loc[1,'krituliu_tipas'] + "  " + all_data.loc[1,'sukibimo_koeficientas'] + "  " + all_data.loc[1,'kelio_danga'] + "  " + all_data.loc[1,'dangos_temperatura'] + " C"
    vievis = all_data.loc[2,'irenginys'] + "    " + all_data.loc[2,'surinkimo_data'] + "\n" + all_data.loc[2,'krituliu_tipas'] + "  " + all_data.loc[2,'sukibimo_koeficientas'] + "  " + all_data.loc[2,'kelio_danga'] + "  " + all_data.loc[2,'dangos_temperatura'] + " C"
    backonys = all_data.loc[3,u'irenginys'] + "    " + all_data.loc[3,'surinkimo_data'] + "\n" + all_data.loc[3,'krituliu_tipas'] + "  " + all_data.loc[3,'sukibimo_koeficientas'] + "  " + all_data.loc[3,'kelio_danga'] + "  " + all_data.loc[3,'dangos_temperatura'] + " C"
    rumsiskes = all_data.loc[4,u'irenginys'] + " " + all_data.loc[4,'surinkimo_data'] + "\n" + all_data.loc[4,'krituliu_tipas'] + "  " + all_data.loc[4,'sukibimo_koeficientas'] + "  " + all_data.loc[4,'kelio_danga'] + "  " + all_data.loc[4,'dangos_temperatura'] + " C"
    
    if (all_data.loc[5,u'irenginys']==u'A. Meškinio tiltas'): all_data.loc[5,u'irenginys'] = u'Meškinio t.'
    meskinis = all_data.loc[5,u'irenginys'] + " " + all_data.loc[5,'surinkimo_data'] + "\n" + all_data.loc[5,'krituliu_tipas'] + "  " + all_data.loc[5,'sukibimo_koeficientas'] + "  " + all_data.loc[5,'kelio_danga'] + "  " + all_data.loc[5,'dangos_temperatura'] + " C"
    
    fortas = all_data.loc[6,u'irenginys'] + "    " + all_data.loc[6,'surinkimo_data'] + "\n" + all_data.loc[6,'krituliu_tipas'] + "  " + all_data.loc[6,'sukibimo_koeficientas'] + "  " + all_data.loc[6,'kelio_danga'] + "  " + all_data.loc[6,'dangos_temperatura'] + " C"
    vilijampole = all_data.loc[7,u'irenginys'] + "    " + all_data.loc[7,'surinkimo_data'] + "\n" + all_data.loc[7,'krituliu_tipas'] + "  " + all_data.loc[7,'sukibimo_koeficientas'] + "  " + all_data.loc[7,'kelio_danga'] + "  " + all_data.loc[7,'dangos_temperatura'] + " C"
    garliava = all_data.loc[8,'irenginys'] + "    " + all_data.loc[8,'surinkimo_data'] + "\n" + all_data.loc[8,'krituliu_tipas'] + "  " + all_data.loc[8,'sukibimo_koeficientas'] + "  " + all_data.loc[8,'kelio_danga'] + "  " + all_data.loc[8,'dangos_temperatura'] + " C"

#--------------------------------------------------

    # clear the frame buffer
    frame_black = [0] * (epd.width * epd.height / 8)
    frame_red = [0] * (epd.width * epd.height / 8)

    # For simplicity, the arguments are explicit numerical coordinates
    # A column
    vilnius_frame = frame_black
    vilnius_fc = UNCOLORED
    vilnius_font_c = COLORED
    if (all_data.loc[0,'kelio_danga'] != 'Sausa'):
        vilnius_frame = frame_red
        vilnius_fc = COLORED
        vilnius_font_c = UNCOLORED
    epd.draw_filled_rectangle(vilnius_frame, -1, -1, 176, 29, vilnius_fc);
    epd.draw_rectangle(vilnius_frame, -1, -1, 176, 29, vilnius_font_c);
    
    didziulis_frame = frame_black
    didziulis_fc = UNCOLORED
    didziulis_font_c = COLORED
    if (all_data.loc[1,'kelio_danga'] != 'Sausa'):
        didziulis_frame = frame_red
        didziulis_fc = COLORED
        didziulis_font_c = UNCOLORED
    epd.draw_filled_rectangle(didziulis_frame, -1, 29, 176, 59, didziulis_fc);
    epd.draw_rectangle(didziulis_frame, -1, 29, 176, 59, didziulis_font_c);
        
    vievis_frame = frame_black
    vievis_fc = UNCOLORED
    vievis_font_c = COLORED
    if (all_data.loc[2,'kelio_danga'] != 'Sausa'):
        vievis_frame = frame_red
        vievis_fc = COLORED
        vievis_font_c = UNCOLORED
    epd.draw_filled_rectangle(vievis_frame, -1, 59, 176, 89, vievis_fc);
    epd.draw_rectangle(vievis_frame, -1, 59, 176, 89, vievis_font_c);
    
    backonys_frame = frame_black
    backonys_fc = UNCOLORED
    backonys_font_c = COLORED
    if (all_data.loc[3,'kelio_danga'] != 'Sausa'):
        backonys_frame = frame_red
        backonys_fc = COLORED
        backonys_font_c = UNCOLORED
    epd.draw_filled_rectangle(backonys_frame, -1, 89, 176, 119, backonys_fc);
    epd.draw_rectangle(backonys_frame, -1, 89, 176, 119, backonys_font_c);

    rumsiskes_frame = frame_black
    rumsiskes_fc = UNCOLORED
    rumsiskes_font_c = COLORED
    if (all_data.loc[4,'kelio_danga'] != 'Sausa'):
        rumsiskes_frame = frame_red
        rumsiskes_fc = COLORED
        rumsiskes_font_c = UNCOLORED
    epd.draw_filled_rectangle(rumsiskes_frame, -1, 119, 176, 149, rumsiskes_fc);
    epd.draw_rectangle(rumsiskes_frame, -1, 119, 176, 149, rumsiskes_font_c);

    meskinis_frame = frame_black
    meskinis_fc = UNCOLORED
    meskinis_font_c = COLORED
    if (all_data.loc[5,'kelio_danga'] != 'Sausa'):
        meskinis_frame = frame_red
        meskinis_fc = COLORED
        meskinis_font_c = UNCOLORED
    epd.draw_filled_rectangle(meskinis_frame, -1, 149, 176, 179, meskinis_fc);
    epd.draw_rectangle(meskinis_frame, -1, 149, 176, 179, meskinis_font_c);

    fortas_frame = frame_black
    fortas_fc = UNCOLORED
    fortas_font_c = COLORED
    if (all_data.loc[6,'kelio_danga'] != 'Sausa'):
        fortas_frame = frame_red
        fortas_fc = COLORED
        fortas_font_c = UNCOLORED
    epd.draw_filled_rectangle(fortas_frame, -1, 179, 176, 209, fortas_fc);
    epd.draw_rectangle(fortas_frame, -1, 179, 176, 209, fortas_font_c);

    vilijampole_frame = frame_black
    vilijampole_fc = UNCOLORED
    vilijampole_font_c = COLORED
    if (all_data.loc[7,'kelio_danga'] != 'Sausa'):
        vilijampole_frame = frame_red
        vilijampole_fc = COLORED
        vilijampole_font_c = UNCOLORED
    epd.draw_filled_rectangle(vilijampole_frame, -1, 209, 176, 239, vilijampole_fc);
    epd.draw_rectangle(vilijampole_frame, -1, 209, 176, 239, vilijampole_font_c);

    garliava_frame = frame_black
    garliava_fc = UNCOLORED
    garliava_font_c = COLORED
    if (all_data.loc[8,'kelio_danga'] != 'Sausa'):
        garliava_frame = frame_red
        garliava_fc = COLORED
        garliava_font_c = UNCOLORED
    epd.draw_filled_rectangle(garliava_frame, -1, 239, 176, 264, garliava_fc);
    epd.draw_rectangle(garliava_frame, -1, 239, 176, 264, garliava_font_c);   


##    # draw strings to the buffer
##    fontBig = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 14)
##    epd.draw_string_at(frame_black, 4, 4, today, fontBig, COLORED)
    
    font = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 11)
    epd.draw_string_at(vilnius_frame, 2, 2, vilnius, font, vilnius_font_c)
    epd.draw_string_at(didziulis_frame, 2, 30, didziulis, font, didziulis_font_c)
    epd.draw_string_at(vievis_frame, 2, 60, vievis, font, vievis_font_c)
    epd.draw_string_at(backonys_frame, 2, 90, backonys, font, backonys_font_c)
    epd.draw_string_at(rumsiskes_frame, 2, 120, rumsiskes, font, rumsiskes_font_c)
    epd.draw_string_at(meskinis_frame, 2, 150, meskinis, font, meskinis_font_c)
    epd.draw_string_at(fortas_frame, 2, 180, fortas, font, fortas_font_c)
    epd.draw_string_at(vilijampole_frame, 2, 210, vilijampole, font, vilijampole_font_c)
    epd.draw_string_at(garliava_frame, 2, 240, garliava, font, garliava_font_c)

    

    
    # display the frames
    epd.display_frame(frame_black, frame_red)

    # display images
##    frame_black = epd.get_frame_buffer(Image.open('remas.bmp'))
##    frame_red = epd.get_frame_buffer(Image.open('vetrunge.bmp'))
##    epd.display_frame(frame_black, frame_red)

    # You can get frame buffer from an image or import the buffer directly:
    #epd.display_frame(imagedata.IMAGE_BLACK, imagedata.IMAGE_RED)


if __name__ == '__main__':
    main()
