module Types exposing (Extension, ExtensionModel, InitialModels(..), Model, Msg(..))

import Json.Encode as JE


type InitialModels
    = Saved
    | Random


type alias ExtensionModel =
    { fitness : Float, definitionSize : Int }


type alias Extension =
    { name : String
    , initialModels : InitialModels
    , models : List ExtensionModel
    }


type alias Model =
    List Extension


type Msg
    = StartExtension Extension
    | SetInitialsModels String InitialModels
    | ShowModels String
    | SetModels JE.Value
