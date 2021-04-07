module Extension exposing (..)

import Html exposing (Html, button, div, h5, i, text)
import Html.Attributes exposing (class, type_)
import Html.Events exposing (onClick)
import Types exposing (Extension, Msg(..))


toHtml_ : Extension -> Html Msg
toHtml_ extension =
    div
        [ class "card mt-3" ]
        [ div
            [ class "card-body" ]
            [ div [ class "row" ]
                [ div
                    [ class "col-sm-8" ]
                    [ h5
                        [ class "card-title" ]
                        [ text extension.name ]
                    ]
                , div
                    [ class "col-sm-4 d-flex justify-content-sm-end" ]
                    [ button
                        [ type_ "button", class "btn btn-outline-success btn-sm align-self-start", onClick (StartExtension extension.name) ]
                        [ i [ class "fas fa-play" ] [], text " Start" ]
                    ]
                ]
            ]
        ]


toHtml : List Extension -> List (Html Msg)
toHtml extensions =
    List.map toHtml_ extensions


fromString : List String -> List Extension
fromString names =
    List.map (\name -> Extension name) names
