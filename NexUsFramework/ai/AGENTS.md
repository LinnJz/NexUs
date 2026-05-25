# NexUs Project Rules

## Scope
These instructions apply to all work in this workspace related to NexUs controls, windows, pages, themes, routing, navigation, styles, models, delegates, and bug fixing.

## Authoritative References
Treat these files as the canonical rules for NexUs control development:

- `E:\NexUs\doc\rules\NexUs-Control-Rules.md`
- `E:\NexUs\doc\rules\NexUs-New-Control-Template-Rules.md`

For any task involving NexUs UI/component code, read the relevant sections of these files before proposing code or implementation steps.

## Mandatory Workflow
Before writing code, first classify the task as one of:
1. Add a new control
2. Add features to an existing control
3. Fix a bug
4. Adjust theme/style/interaction/animation/routing/window behavior

Then determine:
1. Which layer the change belongs to
2. Which Qt base class is semantically correct
3. Which implementation strategy is correct
4. Whether `private` is required
5. Whether `DeveloperComponents` support classes are required
6. Whether theme integration is required
7. Whether animation properties are required
8. Whether the control must integrate with the `NXWindow` page system

Do not start with “just write a QWidget first and patch later”.

## Hard Rules
- Respect NexUs layered architecture:
  - `Source/include` for public API, exported classes, enums, property definitions, macros, base utilities
  - `Source/private` for PIMPL state, private slots, animation objects, helpers, internal logic
  - `Source/DeveloperComponents` for styles, delegates, models, support widgets, platform helpers
  - `Source/*.cpp` for public control assembly and exposed behavior
- Do not place logic in the wrong layer just because it is faster.
- Prefer the closest semantic Qt base class instead of defaulting to `QWidget`.
- When control complexity, state management, theme cache, animation objects, or internal logic grows, use `Private`.
- When drawing/styling/model-view/delegate support is needed, place it in `DeveloperComponents`.
- Follow NexUs property macro conventions and naming conventions.
- Follow NexUs theme integration patterns. Theme changes are not only color changes.
- Keep style ownership, animation ownership, and object lifecycle explicit.
- Avoid patch-style fixes that only make the code “seem to work”.

## New Control Rules
When adding a new control, answer these before coding:
1. What control type is it
2. Which Qt base class should it inherit
3. Which strategy it uses:
   - custom paint
   - proxy style
   - model/view/delegate
   - container/composite
4. Whether `Private` is needed
5. Whether `DeveloperComponents` classes are needed
6. Whether theme hookup is needed
7. Whether animation properties are needed
8. Whether it must integrate into `NXWindow`

Then generate the correct file set and code skeleton accordingly.

## Existing Control Changes
When modifying an existing control, first decide whether the change belongs to:
- public API
- private state/logic
- paint/style layer
- model/data layer
- window shell/page/routing layer

Do not patch behavior into the wrong class.

## Bug Fixing Rules
When fixing bugs, check in order:
1. wrong layer placement
2. theme synchronization
3. style ownership
4. reflection construction / page reconstruction
5. dynamic property protocol
6. routing protocol
7. animation lifecycle

## Output Expectations
For NexUs tasks:
- explain the chosen layer briefly before large edits
- preserve NexUs architecture consistency
- prefer maintainable implementation over short-term hacks
- when uncertain, inspect similar existing NexUs controls before generating new code