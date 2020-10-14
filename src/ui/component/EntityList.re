[@react.component]
let make = () => {
  let (editorState, editorDispatch) =
    React.useContext(Editor_Context.context);
  let (appState, _) = React.useContext(App_Context.context);
  let (_, modalDispatch) = React.useContext(Modal_Context.context);

  <>
    <div className="text-white mb-3"> {React.string("Entity")} </div>
    {appState.entity
     ->Belt.List.map(key =>
         <div key className="flex justify-between" title=key>
           <Button
             onClick={_ => editorDispatch(SelectEntity(key))}
             className={
               (
                 editorState.selectedEntity === key
                   ? "border-dashed border-2" : ""
               )
               ++ " flex-1 text-left"
             }>
             {key->Reshape.Uuid_Util.humanFriendlyEntity->React.string}
           </Button>
           <Button
             onClick={_ => {
               modalDispatch(OpenModal("removeEntity", Entity(key)))
             }}>
             {React.string("x")}
           </Button>
         </div>
       )
     ->(items => React.array(Array.of_list(items)))}
  </>;
};
