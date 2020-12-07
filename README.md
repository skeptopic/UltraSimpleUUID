# UltraSimpleUUID

Extremely simple implementiaton of UUID v4 that is:
* One header file
* Has no dependencies
* Will run on any platform

There is already a more complete UUID implementations out there (https://github.com/mariusbancila/stduuid) but it has dependencies on the operating system and requires C++ 17. Quite often all you want is very simple UUIDs that are "good enough" and that what this for!

## Usage

Simply include `UltraSimpleUUID.h`

`UltraSimpleUUID::generate()` is a convenience function that returns a randomly generated UUID string

`UltraSimpleUUID::Uuid` is a class that represents a single UUID. It default-constructs a Nil UUID. It implements the following funcitons:
* `isNil` is the current UUID [nil](https://en.wikipedia.org/wiki/Universally_unique_identifier#Nil_UUID)
* `randomize` randomizes
* `combine` combines a string into this UUID with a deterministic function.
* `toString`/`fromString` convert this uuid to/from the `xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx` format
* `toBuffer`/`fromBuffer` retrieve/set the 24 bytes that represent this UUID directly

## Requires

C++ 11 standard

## About UUIDs

Basically a [UUID](https://en.wikipedia.org/wiki/Universally_unique_identifier) (Universally Unique IDentifier) is an ID that's "effectivley" guarunteed to be unique without checking a central authority or repository if it is.

The chances of the same UUID being generated independently is very low. See "Randomness" for more information.

There are several versions of UUID, but this one is Version 4, which is based simply on a random number generator. Arguably, Version 1 has a lower chance of UUID collision, but it requires system level information which introduces dependencies.

## Randomness

Each of the 32 characters of the UUID has 62 possible values, which means the number of distinct UUIDS is very large. In theroy, the chance of generating a UUID that matches a specific UUID is `(1/62)^32`, or 4.4e-58, or 1:2.2 Octodecillion or "one in two trillion trillion trillion trillion billion"

However the more UUIDS you have, the greater the chance of collision becomes, so it's not simple to quanitfy the chance of collision in a general case. Theroetically it should only take about 2.71 quintillion UUIDs before the chance of a collision rises above 50%.

However, random number generators in coomputers are never truly random, so I've tried to do some measuring to ensure that it's at least "random enough." I generated 4096 UUIDs and then kept generating UUIDs until all had collided. The average UUID took 11 million tries and the slowest took 40 billion.

This suggests sufficient randomness, but lower than the theoretical value. I have attempted to use the most best randomness C++ 11 has to offer.

## Testing

This includes unit, performance, and randomness tests.

Build and run the Unit tests:

```
mkdir build
cd build
cmake ..
cmake --build . --config Release
ctest -C Release -VV
```

This will also generate the applicaitons `PerformanceTests` and `RandomnessTest`

`PerformanceTests` will measure how long UUID operations take on your machine.
`RandomnessTest` will run the test described in Randomness. Be warned, this is likely to take days to complete.


## License

There is no license. If you want to use this and find it useful, enjoy. Maybe drop me a message to let me know you are. 

## Acknowledgement

I was inspired by https://gist.github.com/fernandomv3/46a6d7656f50ee8d39dc, but wanted some more features and found the randomness of this broken.
