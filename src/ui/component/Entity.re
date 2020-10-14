[@react.component]
let make = (~entity: Reshape.Type.entity, ~humanFriendlyEntity=true) =>
  <div title=entity className="truncate">
    {React.string(
       humanFriendlyEntity ? Reshape.Uuid_Util.humanFriendlyEntity(entity) : entity,
     )}
  </div>;
