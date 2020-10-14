// let getSaveSlots = () => {
//   let saveSlots = Dom.Storage.getItem("saveSlots", Dom.Storage.localStorage);

//   switch(saveSlots) {
//   | Some(saveSlots) =>
//     Json_Util.Parse.maybeArray(
//       array => Belt.Array.map(array, Json_Util.Parse.stringWithDefault("", _)),
//       Js.Json.parseExn(saveSlots)
//     )
//   | None => [| |]
//   };
// };

[@react.component]
let make = () => {
  let (appState, _) = React.useContext(App_Context.context);
  let (_, modalDispatch) = React.useContext(Modal_Context.context);

  // let (saveSlots, _) = React.useState(_ => getSaveSlots());

  <Modal
    id="saveState"
    render={_ => {
      <div>

          <Button
            onClick={_ => {
              SyncState_Util.saveStateInLocalStorage(appState);
              modalDispatch(CloseModal("saveState"));
            }}>
            {React.string("Click to save")}
          </Button>
        </div>
        // {
        //   saveSlots
        //     ->Belt.Array.map(slot =>
        //       <Button
        //         key={slot}
        //         onClick={(_) => modalDispatch(CloseModal("saveState"))}
        //       >
        //         {React.string("Save")}
        //       </Button>
        //     )
        //     ->React.array
        // }
    }}
  />;
};
