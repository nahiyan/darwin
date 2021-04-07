module Types exposing (..)


type InitialModels
    = Saved
    | Random


type alias Extension =
    { name : String
    , initialModels : InitialModels
    }


type alias Model =
    { extensions : List Extension }


type Msg
    = StartExtension Extension
    | SetInitialsModels String InitialModels
