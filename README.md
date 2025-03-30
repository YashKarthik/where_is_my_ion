# Where is my ION???

## LED <-> Driver assignment

1. Driver 1 - 16
    + [x] 1: Conestoga Mall
    + [x] 2: conn to northfield
    + [x] 1: Northfield
    + [x] 3: northfield to r&t
    + [x] 1: R & T
    + [x] 2: r&t to uw
    + [x] 1: UW
    + [x] 1: uw to laurier-waterloo park
    + [x] 1: Laurier-Waterloo Park Stn.
    + [x] 1: laurier-waterloo park to willis way
    + [x] 1: Willis Way Stn.
    + [x] 1: Allen Stn.

2. Driver 2 - 16
    + [x] 1: allen to grh
    + [x] 1: GRH Stn.
    + [x] 2: grh to central
    + [x] 1: Central Stn.
    + [x] 1: Victoria Stn.
    + [x] 1: Queen Stn.
    + [x] 1: Kitchener-Market Stn.
    + [x] 2: kitchener-market to borden
    + [x] 1: Borden Stn.
    + [x] 2: borden to mill
    + [x] 1: Mill Stn.
    + 
    +

3. Driver 3 - 16
    + [x] 3: mill to block line
    + [x] 1: Block Line Stn.
    + [x] 3: block line to fairway
    + [x] 1: Fairway Stn.
-----------reverse
    + [x] 1: Fairway Stn.
    + [x] 3: fairway to block line
    + [x] 1: Block Line Stn.
    + [x] 3: block line to mill

4. Driver 4 - 16
    + [x] 1: Mill Stn.
    + [x] 1: mill to borden
    + [x] 1: Borden Stn.
    + [x] 2: borden to kitchener-market
    + [x] 1: Kitchener-Market Stn.
    + [x] 0: kitchener-market to frederick
    + [x] 1: Frederick Stn.
    + [x] 0: frederick to kitchener city hall
    + [x] 1: Kitchener City Hall Stn.
    + [x] 0: kitchener city hall to central station
    + [x] 1: Central Stn.
    + [x] 2: central to grand river hospital
    + [x] 1: Grand River Hospital Stn.
    + [x] 1: grand river hospital to allen
    + [x] 1: Allen Stn.
    + [x] 0: allen to waterloo public square
    + [x] 1: Waterloo Public Square Stn.
    + [x] 1: waterloo public square to laurier-waterloo park

5. Driver 5 - 14
    + [x] 1: Laurier-Waterloo Park Stn.
    + [x] 1: laurier-waterloo park to university of waterloo
    + [x] 1: University of Waterloo Stn.
    + [x] 2: university of waterloo to research & tech
    + [x] 1: Research & Tech Stn.
    + [x] 3: research & tech to northfield
    + [x] 1: Northfield Stn.
    + [x] 2: northfield to conestoga
    + [x] 1: Conestoga Stn.
    + [x] 1: WIFI (on/off green)
    
    
## Num. LED Calculations

- [x] 16 station leds (c -> f) (blue)
- [x] 16 station leds (f -> c) (blue)
- [x] 21 inter-station leds (c -> f) (white)
- [x] 21 inter-station leds (f -> c) (white)
- [x] 1 red/green => 2 leds (power (ON/OFF))
- [x] 1 green wifi (ON/OFF)

= 78/80 used

### Inter-station LEDs

Conestoga to Fairway
- Conestoga to Northfield: 2 leds
- Northfield to Research & Tech: 3 leds
- Research & Tech to University of Waterloo: 2 leds
- University of Waterloo to Laurier-Waterloo Park: 1 led
- Laurier-Waterloo Park to Willis Way: 1 led
- Willis Way to Allen: 0 led
- Allen to Grand River Hospital: 1 led
- Grand River Hospital to Central Station: 2 leds
- Central Station to Victoria Park (0 led)
- Victoria Park to Queen (0 led)
- Queen to Kitchener-Market (0 led)
- Kitchener Market to Borden (2 led)
- Borden to Mill (1 led)
- Mill to Block Line (3 leds)
- Block Line to Fairway (3 ledss)

Fairway to Conestoga
- Fairway to block line (3 leds)
- Block Line to Mill (3 leds)
- Mill to Borden (1 led)
- Borden to Kitchener-Market (2 led)
- Kitchener-Market to Frederick (0 led)
- Frederick to Kitchener City Hall (0 led)
- Kitchener City Hall to Central Station (0 led)
- Central Station to Grand River Hospital:  2 leds
- Grand River Hospital to Allen: 1 led
- Allen to Waterloo Public Sq: 0 led
- Waterloo Public Sq to Laurier-Waterloo Park: 1 led
- Laurier-Waterloo Park to University of Waterloo to : 1 led
- University of Waterloo to Research & Tech:  2 leds
- Research & Tech to Northfield: 3 leds
- Northfield to Conestoga:  2 leds
