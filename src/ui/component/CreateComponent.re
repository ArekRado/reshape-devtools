let options: Belt.List.t(Select.selectOption) = [
  {value: "Transform", text: "Transform", disabled: false},
  {value: "Sprite", text: "Sprite", disabled: false},
  {value: "FieldFloat", text: "Field Float", disabled: false},
  // {value: "FieldInt", text: "Field Int", disabled: false},
  {value: "FieldVector", text: "Field Vector", disabled: false},
  {value: "Animation", text: "Animation", disabled: false},
  {value: "CollideBox", text: "Collide Box", disabled: false},
  {value: "CollideCircle", text: "Collide Circle", disabled: false},
];

let mapStringToAction = (value, entity) => {
  switch (value) {
  | "Transform" => App_Context.CreateTransform(entity)
  | "Sprite" => App_Context.CreateSprite(entity)
  | "FieldFloat" => App_Context.CreateFieldFloat(entity)
  // | "FieldInt" => App_Context.CreateFieldInt(entity);
  | "FieldVector" => App_Context.CreateFieldVector(entity)
  | "Animation" => App_Context.CreateAnimation(entity)
  | "CollideBox" => App_Context.CreateCollideBox(entity)
  | "CollideCircle" => App_Context.CreateCollideCircle(entity)
  | _ =>
    Js.log3(
      "mapStringToAction has been called with wrong arguments",
      value,
      entity,
    );
    App_Context.CreateTransform(entity);
  };
};

[@react.component]
let make = () => {
  let (editorState, _) = React.useContext(Editor_Context.context);
  let (_, appDispatch) = React.useContext(App_Context.context);

  <Select
    label={React.string("Add component")}
    options
    value=""
    onChange={value =>
      value->mapStringToAction(editorState.selectedEntity)->appDispatch
    }
  />;
};
