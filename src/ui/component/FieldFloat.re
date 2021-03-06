[@react.component]
let make = (~items: Belt.Map.String.t(Reshape.Type.field(float))) => {
  let (_, appDispatch) = React.useContext(App_Context.context);
  let (_, modalDispatch) = React.useContext(Modal_Context.context);
  let (editorState, _) = React.useContext(Editor_Context.context);

  Belt.Map.String.size(items) === 0
    ? React.null
    : <>
        <div className="text-white mt-3"> {React.string("Fields")} </div>
        {items
         ->Belt.Map.String.toArray
         ->Belt.Array.map(((name, field)) =>
             <div key=name className="grid grid-cols-12 gap-1 mb-3 my-1">
               <div className="col-span-5">
                 <Input
                   value=name
                   onChange={event =>
                     appDispatch(
                       SetFieldFloatName(
                         name,
                         ReactEvent.Form.target(event)##value,
                       ),
                     )
                   }
                 />
               </div>
               <div className="col-span-5">
                 <Input
                   type_="number"
                   value={Belt.Float.toString(field.value)}
                   onChange={event =>
                     appDispatch(
                       SetFieldFloatValue(
                         name,
                         ReactEvent.Form.target(event)##value,
                       ),
                     )
                   }
                 />
               </div>
               <Button
                 className="col-span-2"
                 onClick={_ => {modalDispatch(OpenModal(name, None))}}>
                 {React.string("x")}
               </Button>
               <Confirm_Modal
                 id=name
                 title={React.string(
                   "Are you sure you want to remove field float?",
                 )}
                 onAccept={_ =>
                   appDispatch(
                     RemoveFieldFloat(editorState.selectedEntity, name),
                   )
                 }
               />
             </div>
           )
         ->React.array}
      </>;
};
