[@react.component]
let make = (~sprite: option(Reshape.Type.sprite)) => {
  let (appState, appDispatch) = React.useContext(App_Context.context);
  let (editorState, _) = React.useContext(Editor_Context.context);
  let (_, modalDispatch) = React.useContext(Modal_Context.context);
  let entity = editorState.selectedEntity;

  switch (sprite) {
  | Some(sprite) =>
    <>
      <div className="flex justify-between mt-3">
        <div className="text-white"> {React.string("Sprite")} </div>
        <Button
          onClick={_ => {modalDispatch(OpenModal(entity ++ "sprite", None))}}>
          {React.string("x")}
        </Button>
        <Confirm_Modal
          id={entity ++ "sprite"}
          title={React.string("Are you sure you want to remove sprite?")}
          onAccept={_ => appDispatch(RemoveSprite(entity ++ "sprite"))}
        />
      </div>
      <div className="grid grid-cols-12 gap-1 mt-3">
        <div className="text-white mb-3 col-span-12">
          {React.string("Sprite")}
        </div>
        <div className="col-span-4"> {React.string("src")} </div>
        <div className="col-span-8">
          <Select
            value={sprite.src}
            onChange={event =>
              appDispatch(
                SetSpriteSrc(
                  editorState.selectedEntity,
                  ReactEvent.Form.target(event)##value,
                ),
              )
            }
            options={Belt.List.map(appState.asset.sprite, (sprite) =>
              (
                {value: sprite.name, text: sprite.src, disabled: false}: Select.selectOption
              )
            )}
          />
        </div>
        <div className="col-span-12"> <img src={sprite.src} /> </div>
      </div>
    </>
  | None => React.null
  };
};
