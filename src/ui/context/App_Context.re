open Reshape;

let initialState = Type.initialState;

type action =
  | SetState(Type.state)
  | CreateEntity(Type.entity)
  | CreateTransform(Type.entity)
  | CreateSprite(Type.entity)
  | CreateFieldFloat(Type.entity)
  | CreateFieldInt(Type.entity)
  | CreateFieldVector(Type.entity)
  | CreateAnimation(Type.entity)
  | CreateCollideBox(Type.entity)
  | CreateCollideCircle(Type.entity)
  | RemoveEntity(Type.entity)
  | RemoveAnimation(Type.entity, string)
  | RemoveCollideBox(Type.entity, string)
  | RemoveCollideCircle(Type.entity, string)
  | RemoveFieldFloat(Type.entity, string)
  | RemoveSprite(Type.entity)
  | RemoveTransform(Type.entity)
  | SetFieldFloatName(Type.entity, Type.entity)
  | SetFieldFloatValue(Type.entity, float)
  // Sprite
  | SetSpriteSrc(Type.entity, Type.spriteSrc)
  // Transform
  | SetTransformRotation(Type.entity, Type.rotation)
  | SetTransformLocalRotation(Type.entity, Type.rotation)
  | SetTransformScale(Type.entity, Util.Vector.t)
  | SetTransformLocalScale(Type.entity, Util.Vector.t)
  | SetTransformPosition(Type.entity, Util.Vector.t)
  | SetTransformLocalPosition(Type.entity, Util.Vector.t)
  | SetAnimationComponent(
      Type.entity,
      string,
      Type.animatedComponent,
    )
  | SetAnimationKeyframes(
      Type.entity,
      string,
      Belt.List.t(Type.keyframe),
    )
  | SetAnimationKeyframe(
      Type.entity,
      string,
      int,
      Type.keyframe,
    );
// | SetTransformParent(Type.entity, Type.entity)

let reducer = (state, action): Type.state => {
  let newState =
    switch (action) {
    | SetState(state) => state
    | CreateEntity(entity) => Reshape.Entity.create(~entity, ~state)
    | CreateTransform(entity) =>
      Transform_Component.create(~entity, ~state, ())
    | CreateSprite(entity) =>
      Sprite_Component.create(~entity, ~state, ~src="")
    | CreateFieldFloat(entity) =>
      FieldFloat_Component.create(
        ~entity,
        ~state,
        ~name=Uuid_Util.v4(),
        ~value=0.0,
      )
    | CreateFieldInt(_) => state //FieldInt_Component.create(~entity, ~state);
    | CreateFieldVector(_) => state //FieldVector_Component.create(~entity, ~state);
    | CreateAnimation(entity) =>
      Animation_Component.create(
        ~component=Type.FieldFloat(entity, ""),
        ~entity,
        ~state,
        ~name=Uuid_Util.v4(),
        ~keyframes=[],
        (),
      )
    | CreateCollideBox(entity) =>
      CollideBox_Component.create(
        ~entity,
        ~state,
        ~name=Uuid_Util.v4(),
        ~size=Util.Vector.create(1.0, 1.0),
        (),
      )
    | CreateCollideCircle(entity) =>
      CollideCircle_Component.create(
        ~entity,
        ~state,
        ~name=Uuid_Util.v4(),
        ~radius=1.0,
        (),
      )
    | RemoveEntity(entity) => Reshape.Entity.remove(~entity, ~state)
    | RemoveAnimation(entity, name) =>
      Animation_Component.remove(~entity, ~name, ~state)
    | RemoveCollideBox(entity, name) =>
      CollideBox_Component.remove(~entity, ~name, ~state)
    | RemoveCollideCircle(entity, name) =>
      CollideCircle_Component.remove(~entity, ~name, ~state)
    | RemoveFieldFloat(entity, name) =>
      FieldFloat_Component.remove(~entity, ~name, ~state)
    | RemoveSprite(entity) =>
      Sprite_Component.remove(~entity, ~state)
    | RemoveTransform(entity) =>
      Transform_Component.remove(~entity, ~state)

    | SetFieldFloatName(name, newName) => {
        ...state,
        fieldFloat:
          switch (Belt.Map.String.get(state.fieldFloat, name)) {
          | Some(fieldFloat) =>
            Belt.Map.String.set(state.fieldFloat, newName, fieldFloat)
            ->Belt.Map.String.remove(_, name)
          | None => state.fieldFloat
          },
      }
    | SetFieldFloatValue(name, value) => {
        ...state,
        fieldFloat:
          switch (Belt.Map.String.get(state.fieldFloat, name)) {
          | Some(fieldFloat) =>
            Belt.Map.String.set(
              state.fieldFloat,
              name,
              {...fieldFloat, value},
            )
          | None => state.fieldFloat
          },
      }
    | SetSpriteSrc(entity, src) =>
      Sprite_Component.setSrc(~entity, ~src, ~state)

    // Transform

    | SetTransformRotation(entity, rotation) =>
      Transform_Component.setRotation(~entity, ~state, ~rotation)
    | SetTransformLocalRotation(entity, localRotation) =>
      Transform_Component.setLocalRotation(
        ~entity,
        ~state,
        ~localRotation,
      )
    | SetTransformScale(entity, scale) =>
      Transform_Component.setScale(~entity, ~state, ~scale)
    | SetTransformLocalScale(entity, localScale) =>
      Transform_Component.setLocalScale(~entity, ~state, ~localScale)
    | SetTransformPosition(entity, position) =>
      Transform_Component.setPosition(~entity, ~state, ~position)
    | SetTransformLocalPosition(entity, localPosition) =>
      Transform_Component.setLocalPosition(
        ~entity,
        ~state,
        ~localPosition,
      )
    | SetAnimationComponent(entity, name, component) =>
      Animation_Component.setComponent(
        ~name,
        ~entity,
        ~component,
        ~state,
      )
    | SetAnimationKeyframes(entity, name, keyframes) =>
      Animation_Component.setKeyframes(
        ~name,
        ~entity,
        ~keyframes,
        ~state,
      )
    | SetAnimationKeyframe(entity, name, index, keyframe) =>
      Animation_Component.setKeyframe(
        ~name,
        ~entity,
        ~index,
        ~keyframe,
        ~state,
      )
    // | SetTransformParent(entity, parentEntity) => Transform_Component(~entity, ~state, ~rotation);
    };

  SyncState_Util.set(newState, Game);

  newState;
};

type dispatch = action => unit;
type contextType = (Type.state, dispatch);

let contextValue: contextType = (initialState, _ => ignore());
let context = React.createContext(contextValue);

module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };

  let make = React.Context.provider(context);
};
