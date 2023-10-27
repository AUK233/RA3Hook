# Index
New available features in "GameObject"
If element is null, attribute is under root.

## In "Behaviors":
- [SweepingLaserState](#SweepingLaserState)

### SweepingLaserState

| element | attribute | value type | usage description | 
|---|---|---|---|
||Radius|float|negative value is vertical sweeps|
||LaserId|int|when it >= 1000, has the following effects|
|-|-|-|when it is "1000 to 1019", Laser will move length of "Radius" to 0.1x "Radius" of target.|
|-|-|-|when it is "1020 to 1039", Laser makes a circular motion around target with "Radius".|
|-|-|-|when it is "3000 to 3019", Laser will move length of "Radius" from firing source.|
|EndOffset|z|radian|when LaserId >= 500, this value is the rotation angle of the sweep.|
|-|-|-|note that it can't adjust the height in all cases now.|

#### [Return to Index](#index)
