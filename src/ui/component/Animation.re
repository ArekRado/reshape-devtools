open Reshape;

[@react.component]
let make = (~items: Belt.Map.String.t(Type.animation)) => {
  let (appState, appDispatch) = React.useContext(App_Context.context);
  let (_, modalDispatch) = React.useContext(Modal_Context.context);
  let (editorState, _) = React.useContext(Editor_Context.context);

  items
  ->Belt.Map.String.toArray
  ->Belt.Array.map(((key, animation)) => {
      let removeModalId = "remove-" ++ key;

      <div key className="mt-3 flex flex-col">
        <div className="flex justify-between">
          <div className="text-white">
            {React.string("Animation")}
          </div>
          <Button
            onClick={_ => {modalDispatch(OpenModal(removeModalId, None))}}>
            {React.string("x")}
          </Button>
          <Confirm_Modal
            id=removeModalId
            title={React.string("Are you sure you want to remove animation?")}
            onAccept={_ =>
              appDispatch(
                RemoveAnimation(editorState.selectedEntity, animation.name),
              )
            }
          />
        </div>
        <div className="grid grid-cols-12 gap-1 my-1">
          <div className="col-span-4"> {React.string("name")} </div>
          <div className="col-span-8"> {React.string(animation.name)} </div>
          <div className="col-span-4"> {React.string("isPlaying")} </div>
          <div className="col-span-8">
            {React.string(animation.isPlaying ? "true" : "false")}
          </div>
          <div className="col-span-4"> {React.string("currentTime")} </div>
          <div className="col-span-8">
            {React.string(Belt.Float.toString(animation.currentTime))}
          </div>
          <div className="col-span-4"> {React.string("component")} </div>
          <div className="col-span-4">
            {React.string(
               Animation_Modal.mapComponentToText(animation.component),
             )}
          </div>
          <div className="col-span-4">
            {React.string(
               Animation_Modal.getComponentEntity(animation.component),
             )}
          </div>
          <div className="col-span-4"> {React.string("isFinished")} </div>
          <div className="col-span-8">
            {React.string(animation.isFinished ? "true" : "false")}
          </div>
          <div className="col-span-4"> {React.string("wrapMode")} </div>
          <div className="col-span-8">
            {React.string(
               Animation_Modal.wrapModeToString(animation.wrapMode),
             )}
          </div>
        </div>
        <Button
          onClick={_ => {modalDispatch(OpenModal("animationModal", None))}}
          className="">
          {React.string("Edit")}
        </Button>
        <Animation_Modal name={animation.name} />
      </div>;
    })
  ->React.array;
};
