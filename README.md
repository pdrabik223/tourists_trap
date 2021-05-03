# tourists_trap

the third of challenges given to us by gwizdałła

## first thing

all the routes contain the incorrect number of passengers,

> for some weird reason I need to subtract one from all values on the connections

this is because the bus needs one place for the driver or shit.

## The question remains:

If we keep the place for driver of the bus,

do we need to keep one for the tour guide?

okay, so we subtract one from all the vertices,

> next we multiply them by -1, so we have opposites of them,

> that's because we want to have maximal spanning tree, but algorithms are for minimal, so we make data suite our methods

> next we sort the data by number of the passengers

> then we create trees out of sorted data 
> this is the difficult part, because 