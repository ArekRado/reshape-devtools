type syncDirection =
  | Game
  | Editor;

type t = {
  mutable stateForGame: option(Reshape.Type.state),
  mutable stateForEditor: option(Reshape.Type.state),
};

let mutableState: t = {
  stateForGame: Some(Reshape.Type.initialState),
  stateForEditor: Some(Reshape.Type.initialState),
};

let set = (state: Reshape.Type.state, direction: syncDirection): unit => {
  switch (direction) {
  | Game => mutableState.stateForGame = Some(state)
  | Editor => mutableState.stateForEditor = Some(state)
  };

  ();
};

let get = (direction: syncDirection): option(Reshape.Type.state) => {
  switch (direction) {
  | Game =>
    switch (mutableState.stateForGame) {
    | Some(data) =>
      mutableState.stateForGame = None;
      Some(data);
    | None => None
    }
  | Editor =>
    switch (mutableState.stateForEditor) {
    | Some(data) =>
      mutableState.stateForEditor = None;
      Some(data);
    | None => None
    }
  };
};

let getStateFromLocalStorage = (initialState: Reshape.Type.state) => {
  let item = Dom.Storage.getItem("state", Dom.Storage.localStorage);

  switch (item) {
  | Some(item) =>
    let save = item->Js.Json.parseExn->Reshape.Json_Util.Parse.state;

    Some({...save, asset: initialState.asset});
  | None => None;
  };
};

let saveStateInLocalStorage = (state: Reshape.Type.state) => {
  Dom.Storage.setItem(
    "state",
    Reshape.Json_Util.Stringify.state(state),
    Dom.Storage.localStorage,
  );

  ();
};
