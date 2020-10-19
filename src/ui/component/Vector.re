[@react.component]
let make = (~value: Reshape.Util.Vector.t, ~onChange) => {
  let (x, y) = value;

  // <div>
  //   {React.string("x: " ++ Belt.Float.toString(x))}
  //   <br/>
  //   {React.string("y: " ++ Belt.Float.toString(y))}
  // </div>

  <div className="flex">
    <Input
      inputClassName="flex-1"
      type_="number"
      value={Belt.Float.toString(x)}
      onChange={event => onChange((ReactEvent.Form.target(event)##value, y))}
    />
    <Input
      inputClassName="flex-1"
      type_="number"
      value={Belt.Float.toString(y)}
      onChange={event => onChange((x, ReactEvent.Form.target(event)##value))}
    />
  </div>;
};
