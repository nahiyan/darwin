module Extension exposing (..)

import Html exposing (Html, button, div, h5, i, input, label, table, tbody, td, text, th, thead, tr)
import Html.Attributes exposing (attribute, class, id, name, type_)
import Html.Events exposing (onClick)
import Types exposing (Extension, InitialModels(..), Msg(..))


toHtml_ : Extension -> Html Msg
toHtml_ extension =
    let
        models =
            if List.isEmpty extension.models |> not then
                [ div
                    [ class "row" ]
                    [ div
                        [ class "col-md-12" ]
                        [ table [ class "table" ]
                            [ thead []
                                [ tr []
                                    [ th [ attribute "scope" "col" ] [ text "#" ]
                                    , th [ attribute "scope" "col" ] [ text "Fitness" ]
                                    , th [ attribute "scope" "col" ] [ text "Definition Size" ]
                                    ]
                                ]
                            , tbody
                                []
                                (extension.models
                                    |> List.indexedMap
                                        (\index extensionModel ->
                                            tr []
                                                [ td [] [ index + 1 |> String.fromInt |> text ]
                                                , td [] [ extensionModel.fitness |> String.fromFloat |> text ]
                                                , td [] [ extensionModel.definitionSize |> String.fromInt |> text ]
                                                ]
                                        )
                                )
                            ]
                        ]
                    ]
                ]

            else
                []
    in
    div
        [ class "card mt-3" ]
        [ div
            [ class "card-body" ]
            (div [ class "row" ]
                [ div
                    [ class "col-md-3" ]
                    [ h5
                        [ class "card-title" ]
                        [ text extension.name ]
                    ]
                , div
                    [ class "col-md-9 d-flex justify-content-md-end" ]
                    [ input
                        ([ type_ "checkbox"
                         , class "btn-check"
                         , id (extension.name ++ "-show-models")
                         , onClick (ShowModels extension.name)
                         ]
                            ++ (if extension.models |> List.isEmpty then
                                    []

                                else
                                    [ attribute "checked" "" ]
                               )
                        )
                        []
                    , label
                        [ class "btn btn-sm btn-outline-primary me-1"
                        , attribute "for" (extension.name ++ "show-models")
                        , onClick (ShowModels extension.name)
                        ]
                        [ text "Show Models" ]
                    , div
                        [ class "btn-group me-1" ]
                        [ input
                            ([ type_ "radio", class "btn-check", name (extension.name ++ "-initial-models"), id (extension.name ++ "-random-models") ]
                                ++ (if extension.initialModels == Random then
                                        [ attribute "checked" "" ]

                                    else
                                        []
                                   )
                            )
                            []
                        , label [ class "btn btn-outline-primary btn-sm", attribute "for" (extension.name ++ "-random-models"), onClick (SetInitialsModels extension.name Random) ] [ text "Random Models" ]
                        , input
                            ([ type_ "radio", class "btn-check", name (extension.name ++ "-initial-models"), id (extension.name ++ "-saved-models") ]
                                ++ (if extension.initialModels == Saved then
                                        [ attribute "checked" "" ]

                                    else
                                        []
                                   )
                            )
                            []
                        , label [ class "btn btn-outline-primary btn-sm", attribute "for" (extension.name ++ "-saved-models"), onClick (SetInitialsModels extension.name Saved) ] [ text "Saved Models" ]
                        ]
                    , button
                        [ type_ "button", class "btn btn-outline-success btn-sm align-self-start", onClick (StartExtension extension) ]
                        [ i [ class "fas fa-play" ] [], text " Start" ]
                    ]
                ]
                :: models
            )
        ]


toHtml : List Extension -> List (Html Msg)
toHtml extensions =
    List.map toHtml_ extensions


fromString : List String -> List Extension
fromString names =
    List.map (\name -> Extension name Random []) names
