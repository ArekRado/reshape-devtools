[@react.component]
let make =
    (
      ~id,
      ~onAccept,
      ~title,
      ~onCancel=(_: Modal_Context.modal) => {()},
      ~closeOnAction=true,
    ) => {
  let (_, modalDispatch) = React.useContext(Modal_Context.context);

  <Modal
    id
    render={(modal: Modal_Context.modal) => {
      <div className="flex flex-col">
        <div className="text-center"> title </div>
        <div className="flex justify-around mt-6">
          <Button
            onClick={_ => {
              onCancel(modal);
              if (closeOnAction === true) {
                modalDispatch(CloseModal(id));
              };
            }}>
            {React.string("Cancel")}
          </Button>
          <Button
            onClick={_ => {
              onAccept(modal);
              if (closeOnAction === true) {
                modalDispatch(CloseModal(id));
              };
            }}>
            {React.string("Accept")}
          </Button>
        </div>
      </div>
    }}
  />;
};
