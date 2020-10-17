[@bs.val] external requestAnimationFrame : (unit => unit) => unit = "requestAnimationFrame";
[@bs.val] external setTimeout : (unit => unit, int) => float = "setTimeout";

[@bs.module "./asset/example.png"] external example : string = "default";

type gameState = {
    yourState: string,
}

let initialState: Reshape.Type.state =  Reshape.Asset.addSprite(~state=Reshape.Type.initialState, ~src=example, ~name="example");

let rec logic = (state: Reshape.Type.state) => {
  let newState = Reshape.Util.runOneFrame(~state, ())
    ->Debug_System.update(~state=_);

  requestAnimationFrame(() => logic(newState));
};

Reshape.Util.initialize();
logic(initialState); 