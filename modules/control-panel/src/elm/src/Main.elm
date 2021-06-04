port module Main exposing (..)

import Browser
import Extension
import Html exposing (Html, div, h1, text)
import Html.Attributes exposing (class, id)
import Json.Decode as JD
import Json.Encode as JE
import List.Extra exposing (updateIf)
import Types exposing (ExtensionModel, InitialModels(..), Model, Msg(..))



-- PORTS


port startExtension : ( String, String ) -> Cmd msg


port fetchModels : String -> Cmd msg


port receiveModels : (JE.Value -> msg) -> Sub msg


init : List String -> ( Model, Cmd msg )
init extensionNames =
    ( Extension.fromString extensionNames, Cmd.none )


subscriptions : Model -> Sub Msg
subscriptions _ =
    receiveModels SetModels


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
                    updateIf
                        (\extension -> extension.name == extensionName)
                        (\extension ->
                            { extension | initialModels = initialModels }
                        )
                        model
            in
            ( newModel, Cmd.none )

        ShowModels extensionName ->
            let
                ( newExtensions, newCommand ) =
                    List.foldl
                        (\extension ( extensions, command ) ->
                            if extension.name == extensionName then
                                let
                                    newCommand_ =
                                        if extension.models |> List.isEmpty then
                                            fetchModels extension.name

                                        else
                                            Cmd.none

                                    newExtensions_ =
                                        if extension.models |> List.isEmpty |> not then
                                            extensions ++ [ { extension | models = [] } ]

                                        else
                                            extensions ++ [ extension ]
                                in
                                ( newExtensions_, newCommand_ )

                            else
                                ( extensions ++ [ extension ], command )
                        )
                        ( [], Cmd.none )
                        model
            in
            ( newExtensions, newCommand )

        SetModels info ->
            let
                decoder =
                    JD.map2
                        (\extensionName extensionModels ->
                            updateIf
                                (\extension -> extension.name == extensionName)
                                (\extension -> { extension | models = extensionModels })
                                model
                        )
                        (JD.field "extension_name" JD.string)
                        (JD.field "models"
                            (JD.list
                                (JD.map2
                                    ExtensionModel
                                    (JD.field "fitness" JD.float)
                                    (JD.field "definition_size" JD.int)
                                )
                            )
                        )

                newModel =
                    Result.withDefault model (JD.decodeValue decoder info)
            in
            ( newModel, Cmd.none )


view : Model -> Html Msg
view model =
    div [ class "container mt-1" ]
        [ h1 [] [ text "Extensions" ]
        , div [ id "extensions-list" ] (Extension.toHtml model)
        ]
