# LCIOInput

An algorithm wrapper of K4LCIOReader that can cooperate with K4FWCore.

## Dependencies

- [EDM4hep](https://github.com/key4hep/EDM4hep)
- [K4LCIOReader](https://github.com/ihep-sft-group/K4LCIOReader)
- [K4FWCore](https://github.com/key4hep/K4FWCore)

## Build

Now this package can be built as a sub-module of K4FWCore.

1. check out K4FWCore

```shell
git clone https://github.com/key4hep/K4FWCore
```

2. check out LCIOInput and build it together with K4FWCore

```shell
cd K4FWCore
source init.sh
git clone https://github.com/ihep-sft-group/LCIOInput
mkdir build; cd build
cmake ..
make
```

## Contributing

Contributions and bug reports are welcome!
