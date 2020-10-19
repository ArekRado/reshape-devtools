[@react.component]
let make =
    (
      ~value: bool,
      ~onChange,
      ~disabled: option(bool)=?,
      ~label: option(React.element)=?,
      ~containerClassName="",
      ~labelClassName="",
      ~inputClassName="",
      ~id="",
    ) => {
    <div className={containerClassName}>
      {switch (label) {
      | Some(label) => <label htmlFor={id} className={labelClassName}>{label}</label>
      | None => React.null
       }}
      <input
        className={"text-black w-full focus:outline-lightblue " ++ inputClassName}
        type_="checkbox"
        checked={value}
        onChange
        id
        disabled={
          switch (disabled) {
            | Some(disabled) => disabled
            | None => false
            }
        }
      />
    </div>;
};
