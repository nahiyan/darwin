module Types exposing (..)


type alias Extension =
    { name : String }


type alias Model =
    { extensions : List Extension }


type Msg
    = StartExtension String
