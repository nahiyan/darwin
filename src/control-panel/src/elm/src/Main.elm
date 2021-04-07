port module Main exposing (..)

import Browser
import Extension
import Html exposing (Html, div, h1, text)
import Html.Attributes exposing (class, id)
import Types exposing (InitialModels(..), Model, Msg(..))



-- PORTS


port startExtension : ( String, String ) -> Cmd msg


init : List String -> ( Model, Cmd msg )
init extensionNames =
    ( { extensions = Extension.fromString extensionNames }, Cmd.none )


subscriptions : a -> Sub msg
subscriptions _ =
    Sub.none


main : Program (List String) Model Msg
main =
    Browser.element { init = init, update = update, view = view, subscriptions = subscriptions }


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        StartExtension extension ->
            ( model
            , startExtension
                ( extension.name
                , if extension.initialModels == Random then
                    "random"

                  else
                    "saved"
                )
            )

        SetInitialsModels extensionName initialModels ->
            let
                newModel =
                    { model
                        | extensions =
                            model.extensions
                                |> List.map
                                    (\extension ->
                                        if extension.name == extensionName then
                                            { extension | initialModels = initialModels }

                                        else
                                            extension
                                    )
                    }
            in
            ( newModel, Cmd.none )


view : Model -> Html Msg
view model =
    div [ class "container mt-1" ]
        [ h1 [] [ text "Extensions" ]
        , div [ id "extensions-list" ] (Extension.toHtml model.extensions)
        ]
