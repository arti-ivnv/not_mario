

## Assets File Specification

<pre> 
Texture Asset Specification:
---------------------------
Texture N P
    Texture Name        N   std::string
    Texture FilePath    P   std::string

Example: Texture TexStand images/megaman/stand64.png


Animation Asset Specification:
-----------------------------
Animation N T F S
    Animation Name      N   std::string
    Texture Name        T   std::string (refer to an exsiting texture)
    Frame Count         F   int         (number of frames in the Anumation)
    Anim Speed          S   int         (number of game frames between anim frames)

Example: Animation Stand TexStand 1 0


Font Asset Specification:
------------------------
Font N P
    Font Name           N   std::string
    Font File Path      P   std::string
</pre>

---
## Level File Specification
<pre>
Tile Entity Specification:
-------------------------
Tile N GX GY
    Animation Name      N   std::string (Animation asset name for this tile)
    Grid X pos          GX  float
    Grid Y pos          GY  float

Example: Tile Ground 0 0


Decoration Entity Specification:
-------------------------------
Dec N X Y
    Animation Name     N    std::string (Animation asset name for this tile)
    X Position         X    float
    Y Position         Y    float

Example: Dec BushBig 0 1

Player Specification:
--------------------
Player GX GY CW CH SX SY GY B
    Grid X,Y pos       GX, GY   float, float (starting positin of player)
    BoundingBox W|H    CW, CH   float, float
    Left/Right Speed   SX       float
    Jump Speed         SY       float
    Max Speed          SM       float
    Gravity            GY       float
    Bullet Animation   B         std::string (Animation asset to use for bullets)
</pre>
---