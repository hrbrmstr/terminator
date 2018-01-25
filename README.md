
# terminator

Compute Global Terminator (Day/Night) Bands

## Description

Compute global terminator (day/night) bands which can be overlayed as
day and night regions on a ggplot2 world map.

## What’s Inside The Tin

The following functions are implemented:

  - `terminator`: Compute a single termiantor band
  - `terminator_lat_lon`: Generate a full set of terminator frames

## Installation

``` r
devtools::install_github("hrbrmstr/terminator")
```

## Usage

``` r
library(terminator)
library(ggplot2)
library(gganimate) # devtools::install_github("dgrtwo/gganimate")

# current verison
packageVersion("terminator")
```

    ## [1] '0.1.0'

``` r
term_seq <- terminator_lat_lon()

chart <- ggplot(term_seq, aes(frame = frame)) +
  borders("world", colour = "gray90", fill = "gray85") +
  geom_ribbon(aes(lat, ymax = lon), ymin = 90, alpha = 0.2) +
  coord_equal(xlim = c(-180, 190), ylim = c(-58, 85), expand = 0) +
  ggthemes::theme_map()

gganimate(
  chart, 
  interval = 0.1, ani.width=1000, ani.height=600, 
  filename = "terminator-animation.gif"
)
```

![](terminator-animation.gif)
